#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

using namespace v8;
using namespace node;

static RSA *serverPublicKey = NULL;
static RSA *clientPrivateKey = NULL;

static char *privateKey = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIICXQIBAAKBgQCnNh1TKVPy0j3M93vFcN2GLxB1v9uvIxuhwdZsaTVPm9QvENB7\n"
"HSO9mrRg531N5l4r3CYDYBJ1EYIUqH5jzO1MfhOVonJ6kiTxWmAsb2LpHizMjacE\n"
"biHT2KYOekyuwGXN3vbbIvSrbzJjnm22zyScgPnLWDlWFJVwPbG9emdYBwIDAQAB\n"
"AoGAWVevuYSGh2LQhJjjeC6DHrQ8/j1iGyUOGZLhiBF626tuZyt7yZxbQW7M3E40\n"
"BJCHVrYNVZZaplSRhYo15gtT6sK2dvP4yFY4CGm9BEO6Bzgwj12+nvRXZFNoPgkK\n"
"O5mzg00QjTzTrNGcaeDHbglpdZCqdGtbHDaHj3l9lG93JAkCQQDaJ1omoBOFSx8b\n"
"KchhoXz3fx+sfTPtuFEgJJ670/AAKw8OBVaXm6KskPkz6SzgRKfWZrGjjPh2yHoa\n"
"B7h8IOVrAkEAxDhPcFTS5A3kGfYMja05KhgZ1iVqZJIB2TtLSIht5QfoEQIrXX7Z\n"
"EfyirZdqnXV3vVOViU85B2BpYqctGYDi1QJBAIGpzCqTEKc0TEvP3Nsyqd1NQN08\n"
"GaZc3FM3kOVhBhCDh1I1GA/KoLji/ku9dz/hbsS/iywDuRaY+AIKvruArfsCQQCC\n"
"frSOD0uFsZtOjK1Cl75ZuVeNbKq6IcQq9V0W6uyzEbQLg7bVYiP461xBOF8GQvYU\n"
"Kn9bpx1VQ5AytN2whV/VAkAaFBvVgeIo8yeR63uIQ3uX0MMA1lwqyuaishhcWMPh\n"
"rcgtKTDfalkWe6ouZoAH12cJ5E/3SuhOI1mOUXO5kDmK\n"
"-----END RSA PRIVATE KEY-----";
static char *publicKey = "-----BEGIN PUBLIC KEY-----\n"
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDWVlUg1WiagoQ+Kl6hzeUttpdN\n"
"113vjqT1WQQ5sgjHPANoiAJnj6+N0T54Rw6D+/zOQN1rY403CgU9dQANHSBgrFDM\n"
"S89Y4PngWK9i0i+licRjHNTAFWqU2wHtRW3uFKlVna2AaRRypWJ5HmbVL+ox5KRZ\n"
"ZKh5wq8hgAEOugXTfwIDAQAB\n"
"-----END PUBLIC KEY-----";

static Handle<Value> buf2obj(Buffer *buff)
{
	HandleScope scope;
	Local<Object> global_obj = Context::GetCurrent()->Global();
	Local<Function> buffer_creator = Local<Function>::Cast(global_obj->Get(String::New("Buffer")));
	Handle<Value> arg[3] = { buff->handle_, Integer::New(Buffer::Length(buff)), Integer::New(0) };
	Local<Object> act_buffer = buffer_creator->NewInstance(3, arg);
	return scope.Close(act_buffer);
}

static Handle<Value> Test(const Arguments& args)
{
	return String::New("hello world");
}

static Handle<Value> RsaSetup(const Arguments& args)
{
	HandleScope scope;
	serverPublicKey = RSA_new();
	BIO *mem = BIO_new(BIO_s_mem());
	BIO_puts(mem, publicKey);
	PEM_read_bio_RSA_PUBKEY(mem, &serverPublicKey, NULL, NULL);
	BIO_free(mem);

	clientPrivateKey = RSA_new();
	mem = BIO_new(BIO_s_mem());
	BIO_puts(mem, privateKey);
	PEM_read_bio_RSAPrivateKey(mem, &clientPrivateKey, NULL, NULL);
	BIO_free(mem);
	return scope.Close(Integer::New(0));
}

static Handle<Value> RsaDestory(const Arguments& args)
{
	HandleScope scope;
	if (serverPublicKey) {
		RSA_free(serverPublicKey);
		serverPublicKey = NULL;
	}
	if (clientPrivateKey) {
		RSA_free(clientPrivateKey);
		clientPrivateKey = NULL;
	}
	return scope.Close(Integer::New(0));
}

static Handle<Value> RsaPrivateEncrypt(const Arguments& args)
{
	HandleScope scope;

	// Grab the actul data to be written, stuffing it into iov
	if (!Buffer::HasInstance(args[0])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[0]->ToObject();
	char *buffer_data = Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);

	char *buffer = (char *)malloc(1024);
	int len = RSA_private_encrypt(buffer_length, (unsigned char *)buffer_data, (unsigned char *)buffer, clientPrivateKey, RSA_PKCS1_PADDING);
	
	Handle<Value> bufobj = buf2obj(Buffer::New(buffer, len));
	free(buffer);
	return scope.Close(bufobj);
}

static Handle<Value> RsaPrivateDecrypt(const Arguments& args)
{
	HandleScope scope;

	// Grab the actul data to be written, stuffing it into iov
	if (!Buffer::HasInstance(args[0])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[0]->ToObject();
	char *buffer_data = Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);

	char *buffer = (char *)malloc(1024);
	int len = RSA_private_decrypt(buffer_length, (unsigned char *)buffer_data, (unsigned char *)buffer, clientPrivateKey, RSA_PKCS1_PADDING);
	
	Handle<Value> bufobj = buf2obj(Buffer::New(buffer, len));
	free(buffer);
	return scope.Close(bufobj);

}

static Handle<Value> RsaPublicEncrypt(const Arguments& args)
{
	HandleScope scope;

	// Grab the actul data to be written, stuffing it into iov
	if (!Buffer::HasInstance(args[0])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[0]->ToObject();
	char *buffer_data = Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);

	char *buffer = (char *)malloc(1024);
	int len = RSA_public_encrypt(buffer_length, (unsigned char *)buffer_data, (unsigned char *)buffer, clientPrivateKey, RSA_PKCS1_PADDING);
	
	Handle<Value> bufobj = buf2obj(Buffer::New(buffer, len));
	free(buffer);
	return scope.Close(bufobj);

}

static Handle<Value> RsaPublicDecrypt(const Arguments& args)
{
	HandleScope scope;

	// Grab the actul data to be written, stuffing it into iov
	if (!Buffer::HasInstance(args[0])) {
		return ThrowException(Exception::TypeError(
					String::New("Expected a buffer")));
	}

	Local<Object> buffer_obj = args[0]->ToObject();
	char *buffer_data = Buffer::Data(buffer_obj);
	size_t buffer_length = Buffer::Length(buffer_obj);

	char *buffer = (char *)malloc(1024);
	int len = RSA_public_decrypt(buffer_length, (unsigned char *)buffer_data, (unsigned char *)buffer, clientPrivateKey, RSA_PKCS1_PADDING);
	
	Handle<Value> bufobj = buf2obj(Buffer::New(buffer, len));
	return scope.Close(bufobj);

}

extern "C" void
init (Handle<Object> target)
{
	NODE_SET_METHOD(target, "rsaSetup", RsaSetup);
	NODE_SET_METHOD(target, "rsaDestory", RsaDestory);
	NODE_SET_METHOD(target, "rsaPrivateEncrypt", RsaPrivateEncrypt);
	NODE_SET_METHOD(target, "rsaPrivateDecrypt", RsaPrivateDecrypt);
	NODE_SET_METHOD(target, "rsaPublicEncrypt", RsaPublicEncrypt);
	NODE_SET_METHOD(target, "rsaPublicDecrypt", RsaPublicDecrypt);
	NODE_SET_METHOD(target, "test", Test);	
}

