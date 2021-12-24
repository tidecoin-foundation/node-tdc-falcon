{
  'targets': [
    {
      'target_name': 'tdc-falcon-native',
      'sources': [ 'src/tdc_falcon.cc' ,
		                'src/falcon512/aes.c',
		                'src/falcon512/codec.c',
		                'src/falcon512/common.c',
                    'src/falcon512/fft.c',
                    'src/falcon512/fips202.c',
                    'src/falcon512/fpr.c',
                    'src/falcon512/keygen.c',
                    'src/falcon512/pqclean.c',
                    'src/falcon512/randombytes.c',
                    'src/falcon512/rng.c',
                    'src/falcon512/sha2.c',
                    'src/falcon512/sign.c',
                    'src/falcon512/vrfy.c',
      ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}