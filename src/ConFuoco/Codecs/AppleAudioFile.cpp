#include "ConFuoco/Codecs/AppleAudioFile.h"
#if defined(RAINBOW_IOS) || defined(RAINBOW_MAC)

#include <CoreFoundation/CoreFoundation.h>

#include "Common/Debug.h"
#include "Common/IO.h"
#include "ConFuoco/Codecs/OggVorbisAudioFile.h"

#define litlen(literal) sizeof(literal) / sizeof(char) - sizeof(char)

namespace ConFuoco
{
	namespace
	{
		const size_t kAudioBufferSize = 8192;
		const unsigned int kAudioChannelsMono = 1;
		const unsigned int kAudioChannelsStereo = 2;
		const unsigned int kBitsPerChannel = 16;
	}

	AudioFile* AudioFile::Open(const char *const file, const Mode mode)
	{
	#ifndef RAINBOW_IOS
		char id[8];
		Rainbow::IO::FileHandle fh = Rainbow::IO::open(file, Rainbow::IO::ASSET);
		if (fh)
		{
			Rainbow::IO::read(id, 8, fh);
			Rainbow::IO::close(fh);
		}
		if (strncmp(Codecs::kCodecOggVorbis, id, litlen(Codecs::kCodecOggVorbis)) == 0)
			return new OggVorbisAudioFile(file);
		else
	#endif
			return new AppleAudioFile(file, mode);
	}

	AppleAudioFile::AppleAudioFile(const char *const file, const int mode) : ref(nullptr)
	{
	#ifdef RAINBOW_IOS
		NSString *path = Rainbow::IO::open(file, Rainbow::IO::ASSET);
		CFURLRef url = CFURLCreateWithString(
				kCFAllocatorDefault, reinterpret_cast<CFStringRef>(path), nullptr);
	#else
		CFMutableStringRef path = CFStringCreateMutable(kCFAllocatorDefault, 256);
		CFStringAppendCString(path, data_path, kCFStringEncodingUTF8);
		CFStringAppendCString(path, file, kCFStringEncodingUTF8);
		CFURLRef url = CFURLCreateWithFileSystemPath(
				kCFAllocatorDefault, path, kCFURLPOSIXPathStyle, false);
		CFRelease(path);
	#endif
		if (ExtAudioFileOpenURL(url, &this->ref) != noErr)
			R_ERROR("[Rainbow::ConFuoco/AudioToolbox] Failed to open '%s'\n", file);
		CFRelease(url);
		if (!this->ref)
			return;

		UInt32 size = sizeof(this->format);
		memset(&this->format, 0, size);
		if (ExtAudioFileGetProperty(
				this->ref, kExtAudioFileProperty_FileDataFormat, &size,
				&this->format) != noErr)
			R_ERROR("[Rainbow::ConFuoco/AudioToolbox] Failed to retrieve audio format\n");

		FillOutASBDForLPCM(
				this->format,
				this->format.mSampleRate,
				(mode == kAudioFileStream) ? this->format.mChannelsPerFrame : kAudioChannelsMono,
				kBitsPerChannel,
				kBitsPerChannel,
				false,
				false);
		if (ExtAudioFileSetProperty(
				this->ref, kExtAudioFileProperty_ClientDataFormat,
				sizeof(this->format), &this->format) != noErr)
			R_ERROR("[Rainbow::ConFuoco/AudioToolbox] Failed to set client data format\n");
	}

	AppleAudioFile::~AppleAudioFile()
	{
		if (this->ref)
			ExtAudioFileDispose(this->ref);
	}

	int AppleAudioFile::get_channels_impl() const
	{
		if (!this->ref)
			return AudioFile::get_channels();

		return this->format.mChannelsPerFrame;
	}

	int AppleAudioFile::get_rate_impl() const
	{
		if (!this->ref)
			return AudioFile::get_rate();

		return this->format.mSampleRate;
	}

	size_t AppleAudioFile::read_impl(char **dst)
	{
		if (!this->ref)
			return AudioFile::read(dst);

		SInt64 frames = 0;
		UInt32 size = sizeof(frames);
		if (ExtAudioFileGetProperty(
				this->ref, kExtAudioFileProperty_FileLengthFrames, &size,
				&frames) != noErr)
		{
			R_ERROR("[Rainbow::ConFuoco/AudioToolbox] Failed to retrieve audio length\n");
			return 0;
		}
		size = frames * this->format.mBytesPerFrame;
		*dst = new char[size];
		return this->read(*dst, size);
	}

	size_t AppleAudioFile::read_impl(char *dst, const size_t size)
	{
		if (!this->ref)
			return AudioFile::read(dst, size);

		UInt32 frames = size / this->format.mBytesPerFrame;
		AudioBufferList buffer;
		buffer.mNumberBuffers = 1;
		buffer.mBuffers[0].mNumberChannels = this->format.mChannelsPerFrame;
		buffer.mBuffers[0].mDataByteSize = size;
		buffer.mBuffers[0].mData = dst;
		if (ExtAudioFileRead(this->ref, &frames, &buffer) != noErr)
			R_ERROR("[Rainbow::ConFuoco/AudioToolbox] Failed to read <%p>\n",this->ref);
		return frames * this->format.mBytesPerFrame;
	}

	void AppleAudioFile::rewind_impl()
	{
		if (!this->ref)
			return AudioFile::rewind();

		ExtAudioFileSeek(this->ref, 0);
	}
}

#undef litlen
#endif
