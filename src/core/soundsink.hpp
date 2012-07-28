#ifndef CORE_SOUNDSINK_HPP
#define CORE_SOUNDSINK_HPP

#include "types.hpp"
#include "ringbuffer.hpp"

namespace core
{
	struct _soundsink_threading_t;
	struct timestamp_t;
	class SoundSink
	{
	public:
		static const int MAX_TIMEIDX=64;

		typedef core::u32 (*sound_callback_t)(core::s16 *buffer, core::u32 size, void *data, core::u32 *timeidx);
		typedef void (*time_callback_t)(core::u32 skip, void *data);

		SoundSink();
		virtual ~SoundSink();
		virtual int sampleRate() const = 0;

		virtual void setPlaying(bool playing);

		bool isPlaying() const{ return m_playing; }
		void setSoundCallback(sound_callback_t c){ m_soundCallback = c; }
		void setTimeCallback(time_callback_t c){ m_timeCallback = c; }
		void setCallbackData(void *data){ m_callbackData = data; }

		void performSoundCallback(core::s16 *buf, core::u32 sz);
//		void performTimeCallback();
		void applyTime(core::s32 delay_us);

		void blockUntilStopped();
		void blockUntilTimerEmpty();
	private:
		bool _timeloop_read(core::timestamp_t &, u32 &skip);
		void _timeloop();
		static void _timeloop_bootstrap(SoundSink *);
		sound_callback_t m_soundCallback;
		time_callback_t m_timeCallback;
		void *m_callbackData;
		volatile bool m_playing;

		core::u32 m_timeidxsz;
		_soundsink_threading_t *m_threading;
		core::u32 m_timeidx[MAX_TIMEIDX];
		core::RingBuffer m_timeidx_ringbuffer;
	};

	class SoundSinkPlayback : public SoundSink
	{
	public:
		virtual void initialize(unsigned int sampleRate, unsigned int channels, unsigned int latency_ms) = 0;
		virtual void close() = 0;
	};

	core::SoundSink * loadSoundSink(const char *name);
}

#endif
