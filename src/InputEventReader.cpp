#include"../include/stdafx.h"

bool AreEventsContinuous(InputEvent*, InputEvent*);

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

void InputEventReader::Kill()
{
	readerState = ReaderState::Exiting;
	printf("readerState set\n");
}

InputEvent * InputEventReader::GetEvent()
{
	if (inputEventQueue.empty())
	{
		return nullptr;
	}

	std::unique_lock<std::mutex> queueLock(eventListMutex);
	InputEvent* ev = inputEventQueue.front();
	inputEventQueue.pop();

	return ev;
}

bool InputEventReader::IsExiting() 
{
	return readerState == ReaderState::Exiting;
}

void InputEventReader::ReadLoop() 
{
	int fd;
	for(int i = 0; i < 10; i++)
	{
		// Test devices using evtest /dev/input/eventX
		// Open device
		char eventFileName[128];
		sprintf(eventFileName, "/dev/input/event%d", i);
		fd = open(eventFileName, O_RDONLY);
		if (fd == -1)
		{
			fprintf(stderr, "%s0\n", strerror(errno));
			fprintf(stderr, "%s is not a valid device\n", eventFileName);
		}
		else
		{
			struct input_id device_info{};
			ioctl(fd, EVIOCGID, &device_info);
			printf("File: %s  Vendor: %04hx  Product: %04hx  Version: %04hx\n", eventFileName, device_info.vendor, device_info.product, device_info.version);
			if (device_info.vendor == VENDOR_ID_3M && device_info.product == PRODUCT_ID_TOUCH_SCREEN)
			{
				break;
			}

			close(fd);
			fd = -1;
		}
	}

	// If the above for loop completed with fd == -1, then no 3M Touchscreen is attached
	while (fd == -1 && !IsExiting())
	{
		// Sleep for 10s so we dont burn cpu busy waiting on nothing.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	enum FiniteStates { Nothing = 0, Down = 1, Hold = 2 };
	int currentState = Nothing;
	std::vector<InputEvent*> eventChain;
	while (!IsExiting())
	{
		// remember to always discard drag events that are not similar to previous event
		InputEvent* eventPointer = ReadEvent(fd);
		if (eventPointer == nullptr)
		{
			continue;
		}
		else
		{
			std::unique_lock<std::mutex> queueLock(eventListMutex);
			inputEventQueue.push(eventPointer);
		}
	}

	printf("ReadLoop is exiting\n");
	if (fd >= 0)
	{
		close(fd);
	}
}

InputEvent* InputEventReader::ReadEvent(int fd)
{
	int xValue = -1;
	int yValue = -1;
	int buttonTouch = 2;
	struct input_event ev{};
	const size_t ev_size = sizeof(struct input_event);

	while(!IsExiting())
	{
		const size_t size = read(fd, &ev, ev_size);
		if (size < ev_size)
		{
			close(fd);
			return nullptr;
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
					auto inEvent = new InputEvent(xValue, yValue, static_cast<InputEvent::FingerState>(buttonTouch), eventMillis);
					if (inEvent->PositionX < 0 || inEvent->PositionX > DisplayMain::SCREEN_WIDTH || inEvent->PositionY < 0 || inEvent->PositionY > DisplayMain::SCREEN_HEIGHT)
					{
						delete inEvent;
					}
					else
					{
						//printf("rawX = %d, rawY = %d, Type = %d\n", xValue, yValue, buttonTouch);
						return inEvent;
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

bool AreEventsContinuous(InputEvent* prev, InputEvent* curr)
{
	int deltaX = abs(prev->PositionX - curr->PositionX);
	int deltaY = abs(prev->PositionY - curr->PositionY);
	int deltaT = abs(curr->EventTime - prev->EventTime);

	if (curr->State == InputEvent::Up)
	{
		// If an event is an Up, it has no coordinates
		return (deltaT <= 250);
	}

	return (deltaX <= 5 && deltaY <= 5 && deltaT <= 250);
}
