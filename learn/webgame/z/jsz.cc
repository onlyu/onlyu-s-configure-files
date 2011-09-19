#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include "z.h"
#include "zopen.h"
#include <vector>

using namespace v8;
using namespace node;

static std::vector<zreader_t *> zreader_array; 

static void dump_buffer(unsigned char *buff, int len)
{
	for (int i=0; i < len; i++) {
		printf("%02x ", buff[i]);
	}
	printf("\n");
}

static Handle<Value> buf2obj(Buffer *buff)
{
	HandleScope scope;
	Local<Object> global_obj = Context::GetCurrent()->Global();
	Local<Function> buffer_creator = Local<Function>::Cast(global_obj->Get(String::New("Buffer")));
	Handle<Value> arg[3] = { buff->handle_, Integer::New(Buffer::Length(buff)), Integer::New(0) };
	Local<Object> act_buffer = buffer_creator->NewInstance(3, arg);
	return scope.Close(act_buffer);
}

static Handle<Value> create(const Arguments &args)
{
	HandleScope scope;
	zreader_t *reader = (zreader_t *)malloc(sizeof(*reader));
	zreader_init(reader, 12);
	zreader_array.push_back(reader);
	// printf("%p\n", reader);
	return scope.Close(Int32::New(zreader_array.size() - 1));
}

static Handle<Value> decompress(const Arguments &args)
{
	HandleScope scope;
	if (!args[0]->IsInt32()) {
		return ThrowException(Exception::TypeError(String::New("z destroy argument 0 must be int32")));	
	}
	int index = args[0]->Int32Value();
	if (index >= zreader_array.size() || index < 0) {
		return ThrowException(Exception::RangeError(
					String::New("zreader index out of range!")));
	}
	zreader_t *reader = zreader_array[index];
	printf("%d,%p\n", index, reader);

	if (!Buffer::HasInstance(args[1])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[1]->ToObject();
	char *buffer_data = Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);

	// printf("buffer data:");
	// dump_buffer((unsigned char *)buffer_data, buffer_length);

	// callback
  	if (!args[2]->IsFunction()) {
    	return ThrowException(Exception::TypeError( String::New("Thrid argument must be a callback")));
  	}

  	Local<Function> cb = Local<Function>::Cast(args[2]);

	zreader_read(reader, (const char*)buffer_data, buffer_length);
	printf("packet number %d\n", reader->eobcnt);
	for(int i = 0, start = 0; i < reader->eobcnt; start = reader->eob[i], ++i) {
		char *p = (char*)&reader->buf[start];
		int packetlen = reader->eob[i] - start;
		
		//printf("packet: %d, %p\n", i, p);
		//dump_buffer((unsigned char *)p, packetlen);
		Buffer *buffer_object = Buffer::New(p, packetlen);
		Persistent<Value> obj = Persistent<Value>::New(buf2obj(buffer_object));
		Handle<Value> argv[1] = { obj };
		cb->Call(Context::GetCurrent()->Global(), 1, argv);
		// printf("packet: %d\n", i);
	}
	zreader_clear(reader);
	return scope.Close(Integer::New(0));
}

static Handle<Value> free_(const Arguments &args)
{
	HandleScope scope;

	if (!args[0]->IsInt32()) {
		return ThrowException(Exception::TypeError(String::New("z destroy argument 0 must be int32")));	
	}
	int index = args[0]->Int32Value();
	if (index >= zreader_array.size() || index < 0) {
		return ThrowException(Exception::RangeError(
					String::New("zreader index out of range!")));
	}

	zreader_t *reader = zreader_array[index];
	if (reader) {
		zreader_deinit(reader);
		free(reader);
	}
	zreader_array[index] = 0;
	return scope.Close(Integer::New(0));
}

extern "C" void
init (Handle<Object> target)
{
	NODE_SET_METHOD(target, "create", create);	
	NODE_SET_METHOD(target, "decompress", decompress);
	NODE_SET_METHOD(target, "free", free_);
}

