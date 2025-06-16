# 说明
本程序主要去除了adbd的身份校验机制，让adb能够随时连接到手机。而且程序的运行不需要依赖安卓的运行库，可以做到独立运行，通过修改源码可以自定义功能。
# 编译
修改CMakeLists.txt中的CROSS_COMPILE_PREFIX，然后编译即可。
# 修改
主要的文件是`adb.c`和`services.c`，`adb.c`主要定义了adbd相关线程的初始化，`services.c`则主要定义adbd如何解析从adb接收到的命令。在`adb.c`中`我在handle_online`中定义了当adbd与adb建立连接后点亮手机的led灯，在`services.c`中我则添加了一个命令：当输入`adb shell l:/bin/xxx`时，adbd会execp出一个子程序xxx。   
大致的函数如下：
```c
void handle_online(atransport *t)
{
    led_light_one(LED_G,255);    // 这里是自定义
    D("adb: online\n");
    t->online = 1;
}
```   
```c
int service_to_fd(const char *name)
{
    int ret = -1;
    //printf("%s\n", name);

    if(!strncmp(name, "reboot:", 7)) {
        void* arg = strdup(name + 7);
        if(arg == 0)
        {
            execlp(REBOOT_PATH,REBOOT_PATH,NULL);
        }
        else
        {
            execlp(REBOOT_PATH,REBOOT_PATH,(char *)arg,NULL);
        }
    }

    if(!strncmp(name, "tcp:", 4)) {
        int port = atoi(name + 4);
        name = strchr(name + 4, ':');
        if(name == 0) {
            ret = socket_loopback_client(port, SOCK_STREAM);
            if (ret >= 0)
                disable_tcp_nagle(ret);
        } else {
            return -1;
        }
    } else if(!strncmp("dev:", name, 4)) {
        ret = unix_open(name + 4, O_RDWR);
    } else if(!HOST && !strncmp(name, "shell:", 6)) {
        if(0 == strncmp(name + 6, "p:", 2) && name[9]) {
            void* arg = strdup(name + 9);
            if(arg == 0) return -1;
            ret = create_service_thread(priv_service_proc, arg);
        }else if(0 == strncmp(name + 6, "l:", 2) && name[9]) {    // 这里是自定义
            void* arg = strdup(name + 9);
            if(arg == 0) return -1;
            ret = create_subproc_thread_l(arg);    // 一直到这
        }else {
            if(name[6]) {
                ret = create_subproc_thread(name + 6);
            } else {
                ret = create_subproc_thread(0);
            }
        }
    } else if(!strncmp(name, "sync:", 5)) {
        ret = create_service_thread(file_sync_service, NULL);
    } else if (!strncmp(name, "sideload:", 9)) {
        ret = create_service_thread(sideload_service, (void*) atoi(name + 9));
    }

clean:
    if (ret >= 0) {
        close_on_exec(ret);
    }
    return ret;
}
```
# 提示
如果你需要编译稳定可用的程序，建议将我自定义的地方删除或者修改。
# 注意
在将adbd推送到设备运行之前，你需要检查你的设备支不支持android gadget以及其中的ffs function，方法为查看/sys/class/下是否存在android_usb目录，android_usb目录下是否存在f_ffs。如果存在，你需要在/dev/目录下创建路径usb-ffs/adb，并mount adb -t functionfs /dev/usb-ffs/adb。一般情况下，上述的问题不会出现。
