#include"../include/stdafx.h"

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
	int fd = open("/dev/input/event2", O_RDONLY | O_CLOEXEC);
	if (fd == -1)
	{
		fprintf(stderr, "%s is not a valid device\n", "/dev/input/event2");
		while (true) {};
	}

	int xValue = -1;
	int yValue = -1;
	int buttonTouch = 2;
	struct input_event ev{};
	while (!IsExiting())
	{
		const size_t ev_size = sizeof(struct input_event);

		const size_t size = read(fd, &ev, ev_size);
		if (size < ev_size)
		{
			close(fd);
			return;
		}

		switch (ev.type)
		{
			case EV_SYN:
			{
				// Input event has finished, push onto the queue, only if it was received in the last 500ms.
				auto now = std::chrono::system_clock::now();
				const long nowMillis =std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
				const long eventMillis = (ev.time.tv_sec * 1000) + (ev.time.tv_usec / 1000);
				if (nowMillis - eventMillis <= 500)
				{
					auto inEvent = new InputEvent(xValue, yValue, static_cast<InputEvent::FingerState>(buttonTouch));
					if (inEvent->PositionX < 0 || inEvent->PositionX > DisplayMain::SCREEN_WIDTH || inEvent->PositionY < 0 || inEvent->PositionY > DisplayMain::SCREEN_HEIGHT)
					{
						delete inEvent;
					}
					else
					{
						std::unique_lock<std::mutex> queueLock(eventListMutex);
						inputEventQueue.push(inEvent);
					}
				}

				xValue = -1;
				yValue = -1;
				buttonTouch = 2;
				break;
			}
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
