# vim: ft=python

env=DefaultEnvironment()

env = Environment(
                    CCFLAGS = '-std=c++11 -g -O0'
                 )

client = env.SConscript('src/SConscript.Client', variant_dir='build', duplicate=0, exports = ['env'])
api = env.SConscript('src/SConscript.ClientAPI', variant_dir='build', duplicate=0, exports = ['env'])
server = env.SConscript('src/SConscript.Server', variant_dir='build', duplicate=0, exports = ['env'])
env.Depends(client,api)
env.SConscript('test/SConscript.test', variant_dir='build_test', duplicate=0, exports = ['env'])
