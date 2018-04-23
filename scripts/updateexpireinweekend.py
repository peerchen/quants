#coding:utf-8
import redis
import time

#获取当前时间
def GetCurrentTime() :
	return time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

#更新key的expire time
def UpdateTTL(DataList) :
	for item in DataList:
        	TTL_seconds = r.ttl(item)
        	if TTL_seconds > 0 :
                	update_TTL_seconds = TTL_seconds + 2*24*3600
                	r.expire(item, update_TTL_seconds)

#redis服务器ip
host = '127.0.0.1'
#redis服务器端口
port = 6379

r = redis.StrictRedis(host='localhost', port=6379, db=0)
print GetCurrentTime() + " redis:connected."

TickList = r.keys("*:ticks:*")
UpdateTTL(TickList)
print 'update ticks expire time success.'

InstrumentList = r.keys("*:instrumentinfo")
UpdateTTL(InstrumentList)
print 'update intrumentlist expire time success.'
