#include <napi.h>
#include "falcon512/api.h"

using namespace Napi;

Napi::Value GenerateKeyPair(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  void* argp = NULL;
  size_t length = info.Length();

  if (length > 1) {
       Napi::Error::New(env, "generateKeyPair only accepts 1 argument").
       ThrowAsJavaScriptException();
   return Number();
  }

  if(info[0].IsBuffer()) {
    argp = info[0].As<Napi::Buffer<unsigned char>>().Data();
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }

  uint8_t pubk[897];
  uint8_t privk[1281];

  crypto_sign_keypair(pubk, privk, (uint8_t *)argp);

  Napi::Object obj = Napi::Object::New(env);
  obj.Set("pub", Napi::Buffer<uint8_t>::Copy(env,pubk,897));
  obj.Set("priv", Napi::Buffer<uint8_t>::Copy(env,privk,1281));

  return obj;
}

Napi::Value GenerateKeyPairRandom(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  uint8_t pubk[897];
  uint8_t privk[1281];

  crypto_sign_keypair_random(pubk, privk);

  Napi::Object obj = Napi::Object::New(env);
  obj.Set("pub", Napi::Buffer<uint8_t>::Copy(env,pubk,897));
  obj.Set("priv", Napi::Buffer<uint8_t>::Copy(env,privk,1281));

  return obj;
}

Napi::Value Sign(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  uint8_t* sk = NULL;
  uint8_t* m = NULL;
  size_t siglen;
  size_t mlen;
  size_t length = info.Length();

  if (length !=2) {
       Napi::Error::New(env, "sign only accepts 2 arguments").
       ThrowAsJavaScriptException();
   return Number();
  }

  if(info[0].IsBuffer()) {
    sk = info[0].As<Napi::Buffer<unsigned char>>().Data();    
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }

  if(info[1].IsBuffer()) {
    m = info[1].As<Napi::Buffer<unsigned char>>().Data();
    mlen=info[1].As<Napi::Buffer<unsigned char>>().Length();
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }

  uint8_t sig[690];  

  crypto_sign_signature(sig, &siglen, m, mlen, sk);

  std::vector<char> v(sig, sig+siglen);

  return Napi::Buffer<char>::Copy(env, v.data(), v.size());
}

Napi::Value Verify(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  uint8_t* sig = NULL;
  uint8_t* m = NULL;
  uint8_t* pubkey = NULL;
  size_t siglen;
  size_t mlen;

  int result;
  size_t length = info.Length();

  if (length !=3) {
       Napi::Error::New(env, "verify only accepts 3 arguments").
       ThrowAsJavaScriptException();
   return Number();
  }

  if(info[0].IsBuffer()) {
    pubkey = info[0].As<Napi::Buffer<unsigned char>>().Data();    
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }

  if(info[1].IsBuffer()) {
    m = info[1].As<Napi::Buffer<unsigned char>>().Data();
    mlen = info[1].As<Napi::Buffer<unsigned char>>().Length();
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }
  
  if(info[2].IsBuffer()) {
    sig = info[2].As<Napi::Buffer<unsigned char>>().Data();
    siglen = info[2].As<Napi::Buffer<unsigned char>>().Length();
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }

  result = crypto_sign_verify(sig, siglen, m, mlen, pubkey);

  if(result<0) return Napi::Boolean::New(env,false);
  else return Napi::Boolean::New(env,true);
}

Napi::Value PrivToPub(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  uint8_t* sk = NULL;
  size_t length = info.Length();

  if (length !=1) {
       Napi::Error::New(env, "privToPub only accepts 1 arguments").
       ThrowAsJavaScriptException();
   return Number();
  }

  if(info[0].IsBuffer()) {
    sk = info[0].As<Napi::Buffer<unsigned char>>().Data();    
  } else {
     Napi::Error::New(env, "Argument Must be a Buffer").
       ThrowAsJavaScriptException();
     return Number();
  }

  uint8_t pubkey[897];  

  crypto_priv_to_pub(pubkey, sk);

  std::vector<char> v(pubkey, pubkey+897);

  return Napi::Buffer<char>::Copy(env, v.data(), v.size());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "generateKeypair"),
              Napi::Function::New(env, GenerateKeyPair));
  exports.Set(Napi::String::New(env, "generateKeypairRandom"),
              Napi::Function::New(env, GenerateKeyPairRandom));              
  exports.Set(Napi::String::New(env, "sign"),
              Napi::Function::New(env, Sign));
  exports.Set(Napi::String::New(env, "verify"),
              Napi::Function::New(env, Verify));
  exports.Set(Napi::String::New(env, "privToPub"),
              Napi::Function::New(env, PrivToPub));
  return exports;
}

NODE_API_MODULE(addon, Init)
