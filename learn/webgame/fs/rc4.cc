#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <vector>
#include <openssl/rc4.h>

using namespace v8;
using namespace node;

#define RC4_KEY_LEN 4

static Handle<Value> buf2obj(Buffer *buff)
{
	HandleScope scope;
	Local<Object> global_obj = Context::GetCurrent()->Global();
	Local<Function> buffer_creator = Local<Function>::Cast(global_obj->Get(String::New("Buffer")));
	Handle<Value> arg[3] = { buff->handle_, Integer::New(Buffer::Length(buff)), Integer::New(0) };
	Local<Object> act_buffer = buffer_creator->NewInstance(3, arg);
	return scope.Close(act_buffer);
}

static std::vector<RC4_KEY *> _rc4_keys;

static Handle<Value> rc4_key(const Arguments &args)
{
	HandleScope scope;
	if (!Buffer::HasInstance(args[0])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[0]->ToObject();
	unsigned char *buffer_data = (unsigned char *)Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);

	RC4_KEY *key = (RC4_KEY *)malloc(sizeof(*key));
	RC4_set_key(key, RC4_KEY_LEN, buffer_data);
	_rc4_keys.push_back(key);
	return scope.Close(Integer::New(_rc4_keys.size() -1));
}

static Handle<Value> rc4(const Arguments &args)
{
	HandleScope scope;

	if (!args[0]->IsInt32()) {
		return ThrowException(Exception::TypeError(String::New("need number value")));	
	}
	int index = args[0]->Int32Value();
	if (index >= _rc4_keys.size() || index < 0) {
		return ThrowException(Exception::RangeError(
					String::New("rc4 key index out of range!")));
	}
	
	RC4_KEY *key = _rc4_keys[index];
	if (!key) {
		return ThrowException(Exception::TypeError(String::New("argument 1 not a rc4 key")));
	}

	if (!Buffer::HasInstance(args[1])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[1]->ToObject();
	unsigned char *buffer_data = (unsigned char *)Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);
	
	char *enbuffer = (char *) malloc (buffer_length);
	RC4(key, buffer_length, buffer_data, (unsigned char *)enbuffer);	

	Buffer *buf_obj = Buffer::New(enbuffer, buffer_length);
	return scope.Close(buf2obj(buf_obj));
}

static Handle<Value> rc4_free(const Arguments &args)
{
	HandleScope scope;

	if (!args[0]->IsInt32()) {
		return ThrowException(Exception::TypeError(String::New("z destroy argument 0 must be int32")));	
	}
	int index = args[0]->Int32Value();
	if (index >= _rc4_keys.size() || index < 0) {
		return ThrowException(Exception::RangeError(
					String::New("zreader index out of range!")));
	}
	
	RC4_KEY *key = _rc4_keys[index];
	if (key) {
		free(key);
	}
	_rc4_keys[index] = 0;
	return scope.Close(Integer::New(0));
}

extern "C" void
init (Handle<Object> target)
{
	NODE_SET_METHOD(target, "rc4_key", rc4_key);	
	NODE_SET_METHOD(target, "rc4", rc4);
	NODE_SET_METHOD(target, "rc4_free", rc4_free);
}

