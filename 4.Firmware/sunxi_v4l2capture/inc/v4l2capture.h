
/******************************
 struct v4l2_capability
 {
     __u8 driver[16];       // 驱动名字
     __u8 card[32];         // 设备名字
     __u8 bus_info[32];     // 设备在系统中的位置
     __u32 version;         // 驱动版本号
     __u32 capabilities;    // 设备支持的操作
     __u32 reserved[4];     // 保留字段
 };
-----------------------------------
struct v4l2_fmtdesc
{
    __u32 index;               // 要查询的格式序号，应用程序设置
     enum v4l2_buf_type type;   // 帧类型，应用程序设置
     __u32 flags;               // 是否为压缩格式
     __u8 description[32];      // 格式名称
    __u32 pixelformat;         //所支持的格式
     __u32 reserved[4];         // 保留
 };
-----------------------------------
struct v4l2_format
{
     enum v4l2_buf_type type;          // 帧类型，应用程序设置
     union fmt
    {
        struct v4l2_pix_format pix;   // 视频设备使用
         structv 4l2_window win;
        struct v4l2_vbi_format vbi;
         struct v4l2_sliced_vbi_format sliced;
        __u8 raw_data[200];
     };
};
-----------------------------------

struct v4l2_requestbuffers
{
    __u32 count;                    // 缓冲区内缓冲帧的数目
    enum v4l2_buf_type type;        // 缓冲帧数据格式
    enum v4l2_memorymemory;         // 区别是内存映射还是用户指针方式
    __u32 reserved[2];
};
-----------------------------------
 struct v4l2_buffer
 {
     __u32 index;                    //buffer 序号
     enum v4l2_buf_type type;        //buffer 类型
     __u32 byteused;                 //buffer 中已使用的字节数
     __u32 flags;                    // 区分是MMAP 还是USERPTR
     enum v4l2_field field;
     struct timeval timestamp;       // 获取第一个字节时的系统时间
     struct v4l2_timecode timecode;
     __u32 sequence;                 // 队列中的序号
     enum v4l2_memory memory;        //IO 方式，被应用程序设置
     union m
     {
         __u32 offset;               // 缓冲帧地址，只对MMAP 有效
         unsigned long userptr;
     };
     __u32 length;                   // 缓冲帧长度
     __u32 input;
     __u32 reserved;
 };
-----------------------------------
struct VideoDevice {
   int iFd;
   int iPixelFormat;
   int iWidth;
   int iHeight;
   int iVideoBufCnt;
   int iVideoBufMaxLen;
   int iVideoBufCurIndex;
   unsigned char *pucVideBuf[buf_num];

    // 函数 
    PT_VideoOpr ptOPr;
};

***************************************/


int init_v4l2capture(const char* dev);
int v4l2capture_mmap_buffer();
int v4l2capture_unmap_buffer();
void v4l2capture_release_camera();
int v4l2capture_stream_on();
int v4l2capture_stream_off();
int v4l2capture_write_frame();