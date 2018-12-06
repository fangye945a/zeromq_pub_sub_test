#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zmq.h"
//由于ZMQ是基于CS模型的网络通讯库
//订阅端做服务端、允许一个进程订阅多个频道、但是一个频道只能允许一个进程订阅
//发布端做服务端、允许多个进程订阅同一个频道、但是同一个进程只能订阅一个频道
int main(int argc, char *argv[])
{		
    if(argc >= 4 && !strcmp("-sub",argv[2])) // 如果是订阅
    {
		/*************************** 订阅   ***************************/
		void *context = zmq_ctx_new();
		void *subscriber = zmq_socket(context, ZMQ_SUB);
		
		if(!strcmp("-server",argv[1]))          //作服务端
			zmq_bind(subscriber, "tcp://*:12345");   
		else if(!strcmp("-client",argv[1]))		//作客户端
			zmq_connect(subscriber, "tcp://localhost:12345");
		else
		{
			printf("Please add true param:\n"
			   "For example:\n"
			   "-server -sub [topic1] [...]\n"
			   "-client -sub [topic1] [...]\n"
			   "-server -pub [topic] [msg]\n"
			   "-client -pun [topic] [msg]\n");
			return 0;
		}
			
		int i=0;
		for(i=3; i<argc; i++)
		{
			zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, argv[i], strlen(argv[i]));  //允许订阅多个频道
			printf("Sub topic: %s\n",argv[i]);
		}
		char topic_name[256]={0}; //用于接收订阅的主题名
		char payload[1024]={0};   //用于接收订阅主题的内容
		while(1)
		{
			memset(topic_name,0,sizeof(topic_name));
			memset(payload,0,sizeof(payload));

			int size = zmq_recv (subscriber, topic_name, sizeof(topic_name), 0); //接收订阅的主题名称
		    if (size == -1)
		    {
		    	printf("recv topic error!!\n");
		    }
			size = zmq_recv (subscriber, payload, sizeof(payload), 0); //接收订阅的消息
		    if (size == -1)
		    {
		    	printf("recv payload error!!\n");
		    }
			printf("Topic:%s  Msg:%s\n",topic_name, payload);
		}
		zmq_close(subscriber);    //退出时调用
		zmq_ctx_destroy(context);
		return 0;
		/*************************** End  ***************************/
		
    }
    else if(argc == 5 && !strcmp("-pub",argv[2])) //如果时发布
    {
		/*************************** 发布   ***************************/
		void *context = zmq_ctx_new();
		void *publisher = zmq_socket(context, ZMQ_PUB);

		if(!strcmp("-server",argv[1]))			//作服务端
			zmq_bind(publisher, "tcp://*:12345");
		else if(!strcmp("-client",argv[1])) 	//作客户端
			zmq_connect(publisher, "tcp://localhost:12345");
		else
		{
			printf("Please add true param:\n"
			   "For example:\n"
			   "-server -sub [topic1] [...]\n"
			   "-client -sub [topic1] [...]\n"
			   "-server -pub [topic] [msg]\n"
			   "-client -pun [topic] [msg]\n");
			return 0;
		}
		
		while(1)
		{
			zmq_send (publisher, argv[3], strlen(argv[3]), ZMQ_SNDMORE); //指定要发布消息的主题
			zmq_send (publisher, argv[4], strlen(argv[4]), 0);   //向设置的主题发布消息

			//zmq_send (publisher, "hello", strlen("hello"), ZMQ_SNDMORE); //可发布多个主题的消息
			//zmq_send (publisher, "world", strlen("world"), 0);
			sleep(1);									//每秒发布一次
		}
		
		zmq_close(publisher);		//退出时调用
		zmq_ctx_destroy(context);
		return 0;
		/*************************** End  ***************************/
    }
	else
    {
    	printf("Please add true param:\n"
			   "For example:\n"
			   "-server -sub [topic1] [...]\n"
			   "-client -sub [topic1] [...]\n"
			   "-server -pub [topic] [msg]\n"
			   "-client -pun [topic] [msg]\n");
		return 0;
    }
	return 0;
}
