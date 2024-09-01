#include "v4l2capture.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/mman.h>


//用户层缓冲区保存结构体
typedef struct 
{
    void *start;
    int length;
}BUFTYPE_T;
BUFTYPE_T *buffer = NULL;

unsigned int buf_num = 4;//指定缓冲区个数

int fd;
/**
 * @brief get_v4l2capture_fd 获取相机的文件描述符
 * @return 相机的文件描述符
 */
int get_v4l2capture_fd()
{
    return fd;
}
/**
 * @brief init_v4l2capture 初始化相机设备属性
 * @param dev 设备名称
 * @return 成功返回0,失败返回-1
 */
int init_v4l2capture(const char* dev)
{
    struct v4l2_capability cap;
    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_format fmt;
    fd = open(dev, O_RDWR);
    if(fd < 0){
        printf("open \"%s\" error\n", dev);
        return -1;
    }
 
    /**
     * 查询设备属性
     */
    //struct v4l2_capability cap;
    memset(&cap, 0, sizeof(struct v4l2_capability));

    int ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        printf("VIDIOC_QUERYCAP error\n");
        return -1;
    }
 
    printf("driver : %s\n",cap.driver);
    printf("device : %s\n",cap.card);
    printf("bus : %s\n",cap.bus_info);
    printf("version : %d\n",cap.version);

    if(!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))
    { /*判断是否为视频捕获设备*/
        printf("%s is not a video capture device\n",dev);
        return -1;
    }
    if(cap.capabilities & V4L2_CAP_STREAMING)
    {/*判断是否支持视频流捕获*/
         printf("%s support streaming i/o\n",dev);
    }

    if(cap.capabilities & V4L2_CAP_READWRITE)
    {
         printf("%s support read i/o\n",dev);
    }
 
    //struct v4l2_fmtdesc fmtdesc;
    memset(&fmtdesc, 0, sizeof(struct v4l2_fmtdesc));
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;// camera type要设置为V4L2_BUF_TYPE_VIDEO_CAPTURE
    while(ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
    {
        printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }
 
    /*设置格式*/
   // struct v4l2_format fmt;
     memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//摄像头缓冲
    fmt.fmt.pix.width = 720;
    fmt.fmt.pix.height = 480;
   // fmt.fmt.pix.pixelformat = fmtdesc.pixelformat;
   fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0)
    {
        printf("set foramt:V4L2_PIX_FMT_YUYV failed\n");
        return -1;
    }
    return 0;
}

/**
 * @brief mmap_buffer 分配用户缓冲区内存,并建立内存映射
 * @return 成功返回0，失败返回-1
 */
int v4l2capture_mmap_buffer()
{
    struct v4l2_requestbuffers req;
    buffer = (BUFTYPE_T*)calloc(buf_num, sizeof(BUFTYPE_T));
    if (buffer == NULL) {
        printf("calloc \"frame buffer\" error : Out of memory\n");
        return -1;
    }
 
    //struct v4l2_requestbuffers req;
    req.count = buf_num;                    //帧缓冲数量
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; //视频捕获缓冲区类型
    req.memory = V4L2_MEMORY_MMAP;          //内存映射方式
    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        printf("VIDIOC_REQBUFS fail\n");
        return -1;
    }
 
 
    /*映射内核缓存区到用户空间缓冲区*/
    for(unsigned int i = 0; i < buf_num; ++i)
    {
        /*查询内核缓冲区信息*/
        struct v4l2_buffer v4l2_buf;
        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2_buf.memory = V4L2_MEMORY_MMAP;
        v4l2_buf.index = i;
        if(ioctl(fd , VIDIOC_QUERYBUF, &v4l2_buf) < 0){
            printf("VIDIOC_QUERYBUF failed\n");
            return -1;
        }
 
        /* 建立映射关系
         * 注意这里的索引号，v4l2_buf.index 与 buffer 的索引是一一对应的,
         * 当我们将内核缓冲区出队时，可以通过查询内核缓冲区的索引来获取用户缓冲区的索引号，
         * 进而能够知道应该在第几个用户缓冲区中取数据
         */
        buffer[i].length = v4l2_buf.length;
        buffer[i].start = (char *)mmap(0, v4l2_buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, v4l2_buf.m.offset);
 
        if (MAP_FAILED == buffer[i].start){//若映射失败,打印错误
            printf("mmap failed: %d\n",i);
            return -1;
        }else{
            if (ioctl(fd, VIDIOC_QBUF, &v4l2_buf) < 0){ // 若映射成功则将内核缓冲区入队
                printf("VIDIOC_QBUF failed\n");
                return -1;
            }
        }
    }
    return 0;
}


/**
 * @brief unmap_buffer 解除缓冲区映射
 * @return 成功返回0,失败返回-1
 */
int v4l2capture_unmap_buffer()
{
    /*解除内核缓冲区到用户缓冲区的映射*/
    for(unsigned int i = 0; i < buf_num; i++)
    {
        int ret = munmap(buffer[i].start, buffer[i].length);
        if (ret < 0)
        {
            printf("munmap failed\n");
            return -1;
        }
    }
    free(buffer); // 释放用户缓冲区内存
    return 0;
}


/**
 * @brief release_camera 关闭设备
 */
void v4l2capture_release_camera()
{
    close(fd);
}

/**
 * @brief stream_on 开启视频流
 * @return 成功返回0，失败返回-1
 */
int v4l2capture_stream_on()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
    {
        printf("VIDIOC_STREAMON failed\n");
        return -1;
    }
    return 0;
}

/**
 * @brief stream_off 关闭视频流
 * @return 成功返回0,失败返回-1
 */
int v4l2capture_stream_off()
{
    /*关闭视频流*/
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd,VIDIOC_STREAMOFF,&type) == -1)
    {
        printf("Fail to ioctl 'VIDIOC_STREAMOFF'");
        return -1;
    }
    return 0;
}


/**
 * @brief write_frame 读取一帧图像
 * @return  返回图像帧的索引index,读取失败返回-1
 */
int v4l2capture_write_frame()
{
    struct v4l2_buffer v4l2_buf;
     char buf[256];
     int file_fd;
     int size = 0;
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(fd, VIDIOC_DQBUF, &v4l2_buf) < 0) // 内核缓冲区出队列
    {
        printf("VIDIOC_DQBUF failed, dropped frame\n");
        return -1;
    }
 
    /*
     * 因为内核缓冲区与用户缓冲区建立的映射，所以可以通过用户空间缓冲区直接访问这个缓冲区的数据
     */
    //char buf[256];
    sprintf(buf,"/home/%d.yuv",v4l2_buf.index);
    file_fd = open(buf,O_RDWR | O_CREAT,0600); // 若打开失败则不存储该帧图像
    if(file_fd > 0){
        printf("saving %d images\n",v4l2_buf.index);
        size = write(file_fd,buffer[v4l2_buf.index].start,v4l2_buf.bytesused);
        if(size < 0)
         {
            printf("nodate\n");
         }
        close(file_fd);
    }
 
 
    if (ioctl(fd, VIDIOC_QBUF, &v4l2_buf) < 0) //缓冲区重新入队
    {
        printf("VIDIOC_QBUF failed, dropped frame\n");
        return -1;
    }
    return v4l2_buf.index;
}
