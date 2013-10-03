#shared
writer_files = ['src/zebra-writer.c']
reader_files = ['src/zebra-reader.c']

env = Environment(CC = 'gcc', CCFLAGS=['-Wall'])

#main program
writer = env.Program('build/zebra-writer', writer_files)
reader = env.Program('build/zebra-reader', reader_files)

