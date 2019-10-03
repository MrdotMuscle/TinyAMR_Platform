#!/usr/bin/python3

import serial
import datetime
import time


serialHandle = serial.Serial("/dev/ttyUSB0", 115200)  #初始化串口， 波特率为115200

command = {"MOVE_WRITE":1, "POS_READ":28, "SERVO_MODE_WRITE":29,"LOAD_UNLOAD_WRITE": 31,"SERVO_MOVE_STOP":12}

##
##命令发送
##
def servoWriteCmd(id, cmd, par1 = None, par2 = None):
    buf = bytearray(b'\x55\x55')
    try:
        len = 3   #若命令是没有参数的话数据长度就是3
        buf1 = bytearray(b'')

	## 对参数进行处理
        if par1 is not None:
            len += 2  #数据长度加2
            buf1.extend([(0xff & par1), (0xff & (par1 >> 8))])  #分低8位 高8位 放入缓存
        if par2 is not None:
            len += 2
            buf1.extend([(0xff & par2), (0xff & (par2 >> 8))])  #分低8位 高8位 放入缓存
        buf.extend([(0xff & id), (0xff & len), (0xff & cmd)])
        buf.extend(buf1) #追加参数

	##计算校验和
        sum = 0x00
        for b in buf:  #求和
            sum += b
        sum = sum - 0x55 - 0x55  #去掉命令开头的两个 0x55
        sum = ~sum  #取反
        buf.append(0xff & sum)  #取低8位追加进缓存
        serialHandle.write(buf) #发送
    except Exception as e:
        print(e)



##
##读取位置
##
def readPosition(id):
 
    serialHandle.flushInput() #清空接收缓存
    servoWriteCmd(id, command["POS_READ"]) #发送读取位置命令
    time.sleep(0.00034)  #小延时，等命令发送完毕。不知道是否能进行这么精确的延时的，但是修改这个值的确实会产生影响。
                         #实验测试调到这个值的时候效果最好
 
    time.sleep(0.005)  #稍作延时，等待接收完毕
    count = serialHandle.inWaiting() #获取接收缓存中的字节数
    pos = None
    if count != 0: #如果接收到的数据不空
        recv_data = serialHandle.read(count) #读取接收到的数据
        if count == 8: #如果接收到的数据是8个字节（符合位置读取命令的返回数据的长度）
            if recv_data[0] == 0x55 and recv_data[1] == 0x55 and recv_data[4] == 0x1C :
                #第一第二个字节等于0x55, 第5个字节是0x1C 就是 28 就是 位置读取命令的命令号
                 pos= 0xffff & (recv_data[5] | (0xff00 & (recv_data[6] << 8))) #将接收到的字节数据拼接成完整的位置数据
                 #上面这小段代码我们简化了操作没有进行和校验，只要帧头和命令对了就认为数据无误
                 
    return pos  #返回读取到的位置

                

servoWriteCmd(1, command["LOAD_UNLOAD_WRITE"],0)  #命令马达掉点，使舵机可以用手扭动
start_time = datetime.datetime.now()
for i in range(1000):
    servoWriteCmd(1,command["SERVO_MODE_WRITE"],1,500)
#    try:
        #servoWriteCmd(1,1,0,1000)
        #time.sleep(1)
    pos = readPosition(1) #获取1号舵机的位置
end_time = datetime.datetime.now()
print((end_time - start_time).microseconds)
servoWriteCmd(1, command["SERVO_MOVE_STOP"],0)
servoWriteCmd(1,command["SERVO_MODE_WRITE"],0)
servoWriteCmd(1, command["LOAD_UNLOAD_WRITE"],0)
#    print(pos) #打印位置
#    time.sleep(1) #延时1秒
        
#    except Exception as e:
#        print(e)
#        break
