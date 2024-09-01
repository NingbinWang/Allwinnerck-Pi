#include "app.h"
#include "v4l2capture.h"

int main()
{
    int ret = -1;
    ret = init_v4l2capture("/dev/video0");
    if(ret < 0){
        printf("init_camera error\n");
        return -1;
    }
 
    ret = v4l2capture_mmap_buffer();
    if(ret < 0){
        printf("mmap_buffer error\n");
        return -1;
    }
 
    ret = v4l2capture_stream_on();
    if(ret < 0){
        printf("stream_on error\n");
        return -1;
    }
 
    for(int i=0;i<5;i++)
    {
        v4l2capture_write_frame();
    }
 
    ret = v4l2capture_stream_off();
    if(ret < 0){
        printf("stream_off error\n");
        return -1;
    }
 
    ret = v4l2capture_unmap_buffer();
    if(ret < 0){
        printf("unmap_buffer error\n");
        return -1;
    }
    return 0;
}