#pragma once

class Job;

class WinLoop
{
	PUBLIC WinLoop(bool windowed = true);
	PUBLIC ~WinLoop();

	PUBLIC void Loop();
	PUBLIC void ManualLoop();
	PUBLIC MSG GetLastMessage();

	PRIVATE MSG m_message;
	PRIVATE Job* m_job;
};

