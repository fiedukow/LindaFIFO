# vim: ft=python

env=DefaultEnvironment()

env = Environment(
                    CCFLAGS = '-std=c++11 -g -O0',
                 )

client = env.SConscript('src/SConscript.Client', variant_dir='build', duplicate=0, exports = ['env'])
clientAPI = env.SConscript('src/SConscript.ClientAPI', variant_dir='build', duplicate=0, exports = ['env'])
server = env.SConscript('src/SConscript.Server', variant_dir='build', duplicate=0, exports = ['env'])
serverAPI = env.SConscript('src/SConscript.ServerAPI', variant_dir='build', duplicate=0, exports = ['env'])
namedPipe = env.SConscript('src/SConscript.NamedPipe', variant_dir='build', duplicate=0, exports = ['env'])
parser = env.SConscript('src/SConscript.Parser', variant_dir='build', duplicate=0, exports = ['env'])
env.Depends(client,clientAPI)
env.Depends(server,serverAPI)
env.Depends(server,namedPipe)
env.Depends(client,namedPipe)
env.Depends(server,parser)
tests = env.SConscript('test/SConscript.test', variant_dir='build_test', duplicate=0, exports = ['env'])
env.Depends(tests,serverAPI)
