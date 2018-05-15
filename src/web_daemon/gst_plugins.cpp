#ifdef __linux__

extern "C" {
    void gst_init(int *argc, char **argv[]);
    void gst_plugin_coreelements_register();
    
    void gst_plugin_adder_register();
    void gst_plugin_alsa_register();
    void gst_plugin_app_register();
    void gst_plugin_audioconvert_register();
    void gst_plugin_audiomixer_register();
    void gst_plugin_audiorate_register();
    void gst_plugin_audioresample_register();
    void gst_plugin_audiotestsrc_register();
    void gst_plugin_cdparanoia_register();
    void gst_plugin_encoding_register();
    void gst_plugin_gio_register();
    void gst_plugin_ivorbisdec_register();
    void gst_plugin_libvisual_register();
    void gst_plugin_ogg_register();
    void gst_plugin_opus_register();
    void gst_plugin_opengl_register();
    void gst_plugin_pango_register();
    void gst_plugin_playback_register();
    void gst_plugin_rawparse_register();
    void gst_plugin_subparse_register();
    void gst_plugin_tcp_register();
    void gst_plugin_theora_register();
    void gst_plugin_typefindfunctions_register();
    void gst_plugin_videoconvert_register();
    void gst_plugin_videorate_register();
    void gst_plugin_videoscale_register();
    void gst_plugin_videotestsrc_register();
    void gst_plugin_volume_register();
    void gst_plugin_vorbis_register();
    void gst_plugin_ximagesink_register();
    void gst_plugin_xvimagesink_register();

    // good
    
    void gst_plugin_1394_register(); // Source for video data via IEEE1394 interface
    void gst_plugin_aasink_register(); // ASCII Art video sink
    void gst_plugin_alaw_register(); // ALaw audio conversion routines
    void gst_plugin_alphacolor_register(); // RGBA from/to AYUV colorspace conversion preserving the alpha channel
    void gst_plugin_alpha_register(); // adds an alpha channel to video - constant or via chroma-keying
    void gst_plugin_apetag_register(); // APEv1/2 tag reader
    void gst_plugin_audiofx_register(); // Audio effects plugin
    void gst_plugin_audioparsers_register(); // Parsers for various audio formats
    void gst_plugin_auparse_register(); // parses au streams
    void gst_plugin_autodetect_register(); // Plugin contains auto-detection plugins for video/audio in- and outputs
    void gst_plugin_avi_register(); // AVI stream handling
    void gst_plugin_cacasink_register(); // Colored ASCII Art video sink
    void gst_plugin_cairo_register(); // Cairo-based elements
    void gst_plugin_cutter_register(); // Audio Cutter to split audio into non-silent bits
    void gst_plugin_debug_register(); // elements for testing and debugging
    void gst_plugin_deinterlace_register(); // Deinterlacer
    void gst_plugin_directsound_register(); // DirectSound plugin
    void gst_plugin_dtmf_register(); // DTMF plugins
    void gst_plugin_dv_register(); // DV demuxer and decoder based on libdv (libdv.sf.net)
    void gst_plugin_effectv_register(); // effect plugins from the effectv project
    void gst_plugin_equalizer_register(); // GStreamer audio equalizers
    void gst_plugin_flac_register(); // The FLAC Lossless compressor Codec
    void gst_plugin_flv_register(); // FLV muxing and demuxing plugin
    void gst_plugin_flxdec_register(); // FLC/FLI/FLX video decoder
    void gst_plugin_gdkpixbuf_register(); // GdkPixbuf-based image decoder, overlay and sink
    void gst_plugin_goom2k1_register(); // GOOM 2k1 visualization filter
    void gst_plugin_goom_register(); // GOOM visualization filter
    void gst_plugin_gtk_register(); // Gtk+ sink
    void gst_plugin_icydemux_register(); // Demux ICY tags from a stream
    void gst_plugin_id3demux_register(); // Demux ID3v1 and ID3v2 tags from a file
    void gst_plugin_imagefreeze_register(); // Still frame stream generator
    void gst_plugin_interleave_register(); // Audio interleaver/deinterleaver
    void gst_plugin_isomp4_register(); // ISO base media file format support (mp4, 3gpp, qt, mj2)
    void gst_plugin_jack_register(); // JACK audio elements
    void gst_plugin_jpeg_register(); // JPeg plugin library
    void gst_plugin_lame_register(); // Encode MP3s with LAME
    void gst_plugin_level_register(); // Audio level plugin
    void gst_plugin_matroska_register(); // Matroska and WebM stream handling
    void gst_plugin_monoscope_register(); // Monoscope visualization
    void gst_plugin_mpg123_register(); // mp3 decoding based on the mpg123 library
    void gst_plugin_mulaw_register(); // MuLaw audio conversion routines
    void gst_plugin_multifile_register(); // Reads/Writes buffers from/to sequentially named files
    void gst_plugin_multipart_register(); // multipart stream manipulation
    void gst_plugin_navigationtest_register(); // Template for a video filter
    void gst_plugin_oss4_register(); // Open Sound System (OSS) version 4 support for GStreamer
    void gst_plugin_ossaudio_register(); // OSS (Open Sound System) support for GStreamer
    void gst_plugin_osxaudio_register(); // OSX (Mac OS X) audio support for GStreamer
    void gst_plugin_osxvideo_register(); // OSX native video output plugin
    void gst_plugin_png_register(); // PNG plugin library
    void gst_plugin_pulseaudio_register(); // PulseAudio plugin library
    void gst_plugin_qmlgl_register(); // Qt gl plugin
    void gst_plugin_replaygain_register(); // ReplayGain volume normalization
    void gst_plugin_rtpmanager_register(); // RTP session management plugin library
    void gst_plugin_rtp_register(); // Real-time protocol plugins
    void gst_plugin_rtsp_register(); // transfer data via RTSP
    void gst_plugin_shapewipe_register(); // Shape Wipe transition filter
    void gst_plugin_shout2_register(); // Sends data to an icecast server using libshout2
    void gst_plugin_smpte_register(); // Apply the standard SMPTE transitions on video images
    void gst_plugin_soup_register(); // libsoup HTTP client src/sink
    void gst_plugin_spectrum_register(); // Run an FFT on the audio signal, output spectrum data
    void gst_plugin_speex_register(); // Speex plugin library
    void gst_plugin_taglib_register(); // Tag writing plug-in based on taglib
    void gst_plugin_twolame_register(); // Encode MP2s with TwoLAME
    void gst_plugin_udp_register(); // transfer data via UDP
    void gst_plugin_video4linux2_register(); // elements for Video 4 Linux
    void gst_plugin_videobox_register(); // resizes a video by adding borders or cropping
    void gst_plugin_videocrop_register(); // Crops video into a user-defined region
    void gst_plugin_videofilter_register(); // Video filters plugin
    void gst_plugin_videomixer_register(); // Video mixer
    void gst_plugin_vpx_register(); // VP8 plugin
    void gst_plugin_waveform_register(); // WaveForm API based plugin
    void gst_plugin_wavenc_register(); // Encode raw audio into WAV
    void gst_plugin_wavpack_register(); // Wavpack lossless/lossy audio format handling
    void gst_plugin_wavparse_register(); // Parse a .wav file into raw audio
    void gst_plugin_ximagesrc_register(); // X11 video input plugin using standard Xlib calls
    void gst_plugin_y4menc_register(); // Encodes a YUV frame into the yuv4mpeg format (mjpegtools)

    void gst_plugin_libav_register();
}

void gst_load__default_plugins() {
    gst_init(nullptr, nullptr);
    
    gst_plugin_coreelements_register();
    
    gst_plugin_playback_register();
    
    gst_plugin_libav_register();

    //gst_plugin_adder_register();
    //gst_plugin_alsa_register();
    gst_plugin_app_register();
    gst_plugin_audioconvert_register();
    gst_plugin_audiomixer_register();
    gst_plugin_audiorate_register();
    gst_plugin_audioresample_register();
    gst_plugin_audiotestsrc_register();
    //gst_plugin_cdparanoia_register();
    //gst_plugin_encoding_register();
    //gst_plugin_gio_register();
    //gst_plugin_ivorbisdec_register();
    //gst_plugin_libvisual_register();
    //gst_plugin_ogg_register();
    //gst_plugin_opus_register();
    //gst_plugin_opengl_register();
    //gst_plugin_pango_register();
    //gst_plugin_rawparse_register();
    gst_plugin_subparse_register();
    gst_plugin_tcp_register();
    //gst_plugin_theora_register();
    gst_plugin_typefindfunctions_register();
    gst_plugin_videoconvert_register();
    gst_plugin_videorate_register();
    gst_plugin_videoscale_register();
    gst_plugin_videotestsrc_register();
    gst_plugin_volume_register();
    //gst_plugin_vorbis_register();
    //gst_plugin_ximagesink_register();
    //gst_plugin_xvimagesink_register();
    
    //gst_plugin_1394_register();
    //gst_plugin_aasink_register();
    //gst_plugin_alaw_register();
    //gst_plugin_alphacolor_register();
    //gst_plugin_alpha_register();
    //gst_plugin_apetag_register(); // APEv1/2 tag reader
    gst_plugin_audiofx_register(); // Audio effects plugin
    gst_plugin_audioparsers_register(); // Parsers for various audio formats
    gst_plugin_auparse_register(); // parses au streams
    gst_plugin_autodetect_register(); // Plugin contains auto-detection plugins for video/audio in- and outputs
    //gst_plugin_avi_register(); // AVI stream handling
    //gst_plugin_cacasink_register(); // Colored ASCII Art video sink
    //gst_plugin_cairo_register(); // Cairo-based elements
    //gst_plugin_cutter_register(); // Audio Cutter to split audio into non-silent bits
    //gst_plugin_debug_register(); // elements for testing and debugging
    //gst_plugin_deinterlace_register(); // Deinterlacer
    //gst_plugin_directsound_register(); // DirectSound plugin
    //gst_plugin_dtmf_register(); // DTMF plugins
    //gst_plugin_dv_register(); // DV demuxer and decoder based on libdv (libdv.sf.net)
    //gst_plugin_effectv_register(); // effect plugins from the effectv project
    //gst_plugin_equalizer_register(); // GStreamer audio equalizers
    //gst_plugin_flac_register(); // The FLAC Lossless compressor Codec
    gst_plugin_flv_register(); // FLV muxing and demuxing plugin
    gst_plugin_flxdec_register(); // FLC/FLI/FLX video decoder
    //gst_plugin_gdkpixbuf_register(); // GdkPixbuf-based image decoder, overlay and sink
    //gst_plugin_goom2k1_register(); // GOOM 2k1 visualization filter
    //gst_plugin_goom_register(); // GOOM visualization filter
    //gst_plugin_gtk_register(); // Gtk+ sink
    //gst_plugin_icydemux_register(); // Demux ICY tags from a stream
    //gst_plugin_id3demux_register(); // Demux ID3v1 and ID3v2 tags from a file
    //gst_plugin_imagefreeze_register(); // Still frame stream generator
    //gst_plugin_interleave_register(); // Audio interleaver/deinterleaver
    gst_plugin_isomp4_register(); // ISO base media file format support (mp4, 3gpp, qt, mj2)
    //gst_plugin_jack_register(); // JACK audio elements
    //gst_plugin_jpeg_register(); // JPeg plugin library
    //gst_plugin_lame_register(); // Encode MP3s with LAME
    //gst_plugin_level_register(); // Audio level plugin
    //gst_plugin_matroska_register(); // Matroska and WebM stream handling
    //gst_plugin_monoscope_register(); // Monoscope visualization
    //gst_plugin_mpg123_register(); // mp3 decoding based on the mpg123 library
    gst_plugin_mulaw_register(); // MuLaw audio conversion routines
    //gst_plugin_multifile_register(); // Reads/Writes buffers from/to sequentially named files
    //gst_plugin_multipart_register(); // multipart stream manipulation
    //gst_plugin_navigationtest_register(); // Template for a video filter
    gst_plugin_oss4_register(); // Open Sound System (OSS) version 4 support for GStreamer
    gst_plugin_ossaudio_register(); // OSS (Open Sound System) support for GStreamer
    //gst_plugin_osxaudio_register(); // OSX (Mac OS X) audio support for GStreamer
    //gst_plugin_osxvideo_register(); // OSX native video output plugin
    //gst_plugin_png_register(); // PNG plugin library
    gst_plugin_pulseaudio_register(); // PulseAudio plugin library
    //gst_plugin_qmlgl_register(); // Qt gl plugin
    gst_plugin_replaygain_register(); // ReplayGain volume normalization
    gst_plugin_rtpmanager_register(); // RTP session management plugin library
    gst_plugin_rtp_register(); // Real-time protocol plugins
    //gst_plugin_rtsp_register(); // transfer data via RTSP
    gst_plugin_shapewipe_register(); // Shape Wipe transition filter
    //gst_plugin_shout2_register(); // Sends data to an icecast server using libshout2
    gst_plugin_smpte_register(); // Apply the standard SMPTE transitions on video images
    //gst_plugin_soup_register(); // libsoup HTTP client src/sink
    //gst_plugin_spectrum_register(); // Run an FFT on the audio signal, output spectrum data
    //gst_plugin_speex_register(); // Speex plugin library
    //gst_plugin_taglib_register(); // Tag writing plug-in based on taglib
    //gst_plugin_twolame_register(); // Encode MP2s with TwoLAME
    gst_plugin_udp_register(); // transfer data via UDP
    //gst_plugin_video4linux2_register(); // elements for Video 4 Linux
    gst_plugin_videobox_register(); // resizes a video by adding borders or cropping
    gst_plugin_videocrop_register(); // Crops video into a user-defined region
    gst_plugin_videofilter_register(); // Video filters plugin
    gst_plugin_videomixer_register(); // Video mixer
    //gst_plugin_vpx_register(); // VP8 plugin
    //gst_plugin_waveform_register(); // WaveForm API based plugin
    gst_plugin_wavenc_register(); // Encode raw audio into WAV
    //gst_plugin_wavpack_register(); // Wavpack lossless/lossy audio format handling
    gst_plugin_wavparse_register(); // Parse a .wav file into raw audio
    //gst_plugin_ximagesrc_register(); // X11 video input plugin using standard Xlib calls
    //gst_plugin_y4menc_register(); // Encodes a YUV frame into the yuv4mpeg format (mjpegtools)
}

#endif
