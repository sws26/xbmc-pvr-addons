/*
 *      vdr-plugin-vnsi - XBMC server plugin for VDR
 *
 *      Copyright (C) 2010 Alwin Esch (Team XBMC)
 *
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef VNSI_DEMUXER_H264_H
#define VNSI_DEMUXER_H264_H

#include "parser.h"

class cBitstream;

// --- cParserH264 -------------------------------------------------

class cParserH264 : public cParser
{
private:
  typedef struct h264_private
  {
    struct SPS
    {
      int frame_duration;
      int cbpsize;
      int pic_order_cnt_type;
      int frame_mbs_only_flag;
      int log2_max_frame_num;
      int log2_max_pic_order_cnt_lsb;
      int delta_pic_order_always_zero_flag;
    } sps[256];

    struct PPS
    {
      int sps;
      int pic_order_present_flag;
    } pps[256];

    struct VCL_NAL
    {
      int frame_num; // slice
      int pic_parameter_set_id; // slice
      int field_pic_flag; // slice
      int bottom_field_flag; // slice
      int delta_pic_order_cnt_bottom; // slice
      int delta_pic_order_cnt_0; // slice
      int delta_pic_order_cnt_1; // slice
      int pic_order_cnt_lsb; // slice
      int idr_pic_id; // slice
      int nal_unit_type;
      int nal_ref_idc; // start code
      int pic_order_cnt_type; // sps
    } vcl_nal;

  } h264_private_t;

  typedef struct mpeg_rational_s {
    int num;
    int den;
  } mpeg_rational_t;

  enum
  {
    NAL_SLH     = 0x01, // Slice Header
    NAL_SEI     = 0x06, // Supplemental Enhancement Information
    NAL_SPS     = 0x07, // Sequence Parameter Set
    NAL_PPS     = 0x08, // Picture Parameter Set
    NAL_AUD     = 0x09, // Access Unit Delimiter
    NAL_END_SEQ = 0x0A  // End of Sequence
  };

  uint32_t        m_StartCode;
  bool            m_NeedIFrame;
  bool            m_NeedSPS;
  bool            m_NeedPPS;
  int             m_Width;
  int             m_Height;
  int             m_FPS;
  int             m_FpsScale;
  mpeg_rational_t m_PixelAspect;
  int             m_FrameDuration;
  h264_private    m_streamData;
  int             m_vbvDelay;       /* -1 if CBR */
  int             m_vbvSize;        /* Video buffer size (in bytes) */
  int64_t         m_DTS;
  int64_t         m_PTS;


  int Parse_H264(uint32_t startcode, int buf_ptr, bool &complete);
  bool Parse_PPS(uint8_t *buf, int len);
  bool Parse_SLH(uint8_t *buf, int len, h264_private::VCL_NAL &vcl);
  bool Parse_SPS(uint8_t *buf, int len);
  bool IsFirstVclNal(h264_private::VCL_NAL &vcl);

public:
  cParserH264(int pID, cTSStream *stream);
  virtual ~cParserH264();

  virtual void Parse(sStreamPacket *pkt);
  virtual void Reset();
};


#endif // VNSI_DEMUXER_H264_H
