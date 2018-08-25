#include<stdafx.h>

// Static member initializations
InputEventReader::ReaderState InputEventReader::readerState = Uninitialized;
std::mutex InputEventReader::eventListMutex;
std::queue<InputEvent*> InputEventReader::inputEventQueue;

void InputEventReader::Start() 
{
	if (readerState != ReaderState::Uninitialized)
	{
		return;
	}

	readerState = ReaderState::Reading;

	ReadLoop();
}

InputEvent * InputEventReader::GetEvent()
{
	if (inputEventQueue.empty())
	{
		return nullptr;
	}

	eventListMutex.lock();
	InputEvent* ev = inputEventQueue.front();
	inputEventQueue.pop();
	eventListMutex.unlock();

	return ev;
}

bool InputEventReader::IsExiting() 
{
	return readerState == ReaderState::Exiting;
}

void InputEventReader::ReadLoop() 
{
	// Open device
	int fd = open("/dev/input/event2", O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "%s is not a valid device\n");
		return;
	}

	int xValue = -1;
	int yValue = -1;
	int buttonTouch = 2;
	struct input_event ev;
	while (!IsExiting())
	{
		const size_t ev_size = sizeof(struct input_event);
		ssize_t size;

		size = read(fd, &ev, ev_size);
		if (size < ev_size)
		{
			close(fd);
			return;
		}

		switch (ev.type)
		{
			case EV_SYN:
				// Input event has finished, push onto the queue
				eventListMutex.lock();
				inputEventQueue.push(new InputEvent(xValue, yValue, static_cast<InputEvent::FingerState>(buttonTouch)));
				eventListMutex.unlock();

				xValue = -1;
				yValue = -1;
				buttonTouch = 2;
				break;

			case EV_KEY:
				buttonTouch = ev.value;
				break;

			case EV_ABS:
				if (ev.code == ABS_X)
				{
					yValue = ev.value;
				}
				else if (ev.code == ABS_Y)
				{
					xValue = ev.value;
				}
				break;

			default:
				break;
		}
	}
}