import os

# simply clone entire environment
env = Environment(ENV = os.environ)

# set up user-specific environment variables
# IMPORTANT: THESE VARIABLES HAVE TO BE SET ACCORDING TO YOUR SYSTEM!
LLVM_INSTALL_DIR = env['ENV']['LLVM_INSTALL_DIR']
PACKETIZER_INSTALL_DIR = env['ENV']['PACKETIZER_INSTALL_DIR']


# build variables
debug         = ARGUMENTS.get('debug', 0)
debug_runtime = ARGUMENTS.get('debug-runtime', 0)
profile       = ARGUMENTS.get('profile', 0)
use_openmp    = ARGUMENTS.get('openmp', 0)
num_threads   = ARGUMENTS.get('threads', 0)
packetize     = ARGUMENTS.get('packetize', 0)
split         = ARGUMENTS.get('split', 0)

compile_dynamic_lib_driver = ARGUMENTS.get('dynamic', 0)

if int(debug) and int(use_openmp):
	print "\nWARNING: Using OpenMP in debug mode might lead to unknown behaviour!\n"

# find out if we are on windows
isWin = env['PLATFORM'] == 'win32' # query HOST_OS or TARGET_OS instead of PLATFORM?
isDarwin = env['PLATFORM'] == 'darwin' # query HOST_OS or TARGET_OS instead of PLATFORM?
is32Bit = env['TARGET_ARCH'] == 'x86'
#print env['HOST_OS']
#print env['HOST_ARCH']
#print env['TARGET_OS']
#print env['TARGET_ARCH']
#print env['PLATFORM']
#print env['ENV']

# set up compiler
if isWin:
	env['CC'] = 'cl'
	env['CXX'] = 'cl'
else:
	#env['CC'] = 'clang'    # no -fopenmp :(
	#env['CXX'] = 'clang++' # no -fopenmp :(
	env['CC'] = 'gcc'
	env['CXX'] = 'g++'

# query llvm-config
if isWin:
	llvm_vars = env.ParseFlags([
	"-I"+os.path.join(LLVM_INSTALL_DIR, 'include')+" -L"+os.path.join(LLVM_INSTALL_DIR, 'lib')+
	" -D_GNU_SOURCE -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -lLLVMObject -lLLVMMCJIT -lLLVMMCDisassembler -lLLVMLinker -lLLVMipo -lLLVMInterpreter -lLLVMInstrumentation -lLLVMJIT -lLLVMExecutionEngine -lLLVMBitWriter -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMX86Info -lLLVMAsmParser -lLLVMArchive -lLLVMBitReader -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMMCParser -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMCore -lLLVMMC -lLLVMSupport -lshell32 -ladvapi32"
	])
else:
	llvm_vars = env.ParseFlags('!llvm-config --cflags --ldflags --libs')

# set up CXXFLAGS
if isWin:
	# /Wall gives LOTS of warnings -> leave at default warning level
	cxxflags = env.Split("/MDd /EHsc")+llvm_vars.get('CCFLAGS')
else:
	cxxflags = env.Split("-Wall -pedantic -Wno-long-long -msse3")+llvm_vars.get('CCFLAGS')

if int(debug) or int(debug_runtime):
	if isWin:
		cxxflags=cxxflags+env.Split("/Od /Zi")
	else:
		cxxflags=cxxflags+env.Split("-O0 -g")

if int(debug):
	cxxflags=cxxflags+env.Split("-DDEBUG -D_DEBUG")

if int(debug_runtime):
	cxxflags=cxxflags+env.Split("-DDEBUG_RUNTIME")

if not int(debug) and not int(debug_runtime):
	if isWin:
		cxxflags=cxxflags+env.Split("/Ox -DNDEBUG")
	else:
		cxxflags=cxxflags+env.Split("-O3 -DNDEBUG")

if int(profile):
	if isWin:
		cxxflags=cxxflags+env.Split("/Od /Zi")
	else:
		cxxflags=cxxflags+env.Split("-O0 -g")
	# disabled until we have 64bit VTune libraries
	#cxxflags=cxxflags+env.Split("-g -DPACKETIZED_OPENCL_ENABLE_JIT_PROFILING")
	#compile_dynamic_lib_driver=0

if int(use_openmp):
	if isWin:
		cxxflags=cxxflags+env.Split("/DPACKETIZED_OPENCL_USE_OPENMP /openmp")
	else:
		cxxflags=cxxflags+env.Split("-DPACKETIZED_OPENCL_USE_OPENMP -fopenmp")
		env.Append(LINKFLAGS = env.Split("-fopenmp"))
	if int(num_threads):
		cxxflags=cxxflags+env.Split("-DPACKETIZED_OPENCL_NUM_CORES="+num_threads)

if not int(packetize):
	cxxflags=cxxflags+env.Split("-DPACKETIZED_OPENCL_NO_PACKETIZATION")

if int(split):
	cxxflags=cxxflags+env.Split("-DPACKETIZED_OPENCL_SPLIT_EVERYTHING")

if int(packetize) and not int(compile_dynamic_lib_driver):
	cxxflags=cxxflags+env.Split("-DPACKETIZER_STATIC_LIBS")


env.Append(CXXFLAGS = cxxflags)

# set up paths
# LLVM_INSTALL_DIR and PACKETIZER_INSTALL_DIR are supposed to be set by setupenv.sh
env.Append(CPPPATH = env.Split("include"))
env.Append(CPPPATH = llvm_vars.get('CPPPATH'))
env.Append(CPPPATH = [os.path.join(LLVM_INSTALL_DIR, 'include')])
env.Append(CPPPATH = [os.path.join(PACKETIZER_INSTALL_DIR, 'include')])

env.Append(LIBPATH = [os.path.join(LLVM_INSTALL_DIR, 'lib')])
env.Append(LIBPATH = [os.path.join(PACKETIZER_INSTALL_DIR, 'lib')])
env.Append(LIBPATH = llvm_vars.get('LIBPATH'))
env.Append(LIBPATH = env.Split("lib"))
if is32Bit:
	env.Append(LIBPATH = env.Split("lib/x86"))
else:
	env.Append(LIBPATH = env.Split("lib/x86_64"))

# set up libraries
# glut and GLEW are not required for all, but this is easier :P
driverLibs = llvm_vars.get('LIBS') + env.Split('Packetizer')
if isWin:
	# get glut from http://www.idfun.de/glut64/
	if int(compile_dynamic_lib_driver):
		appLibs = env.Split('OpenCL SDKUtil glut32 glew32')
	else:
		appLibs = env.Split('PacketizedOpenCL SDKUtil glut32 glew32')
else:
	if int(compile_dynamic_lib_driver):
		appLibs = env.Split('OpenCL SDKUtil glut GLEW')
		# The following will only work as soon as Apple uses ICD etc. (OpenCL 1.1)
		#if isDarwin:
			#appLibs = env.Split('SDKUtil glut GLEW')
		#else:
			#appLibs = env.Split('OpenCL SDKUtil glut GLEW')
	else:
		appLibs = env.Split('PacketizedOpenCL SDKUtil glut GLEW')

# disabled until we have 64bit VTune libraries
#if int(profile):
	#driverLibs=driverLibs+env.Split("dl pthread JITProfiling")
	#env.Append(CPPPATH = [os.path.join(env['ENV']['VTUNE_GLOBAL_DIR'], 'analyzer/include')])
	#env.Append(LIBPATH = [os.path.join(env['ENV']['VTUNE_GLOBAL_DIR'], 'analyzer/bin')])

# build Packetized OpenCL Driver
driverSrc = env.Glob('src/*.cpp')
if int(compile_dynamic_lib_driver):
	PacketizedOpenCL = env.SharedLibrary(target='lib/PacketizedOpenCL', source=driverSrc, CPPDEFINES=llvm_vars.get('CPPDEFINES'), LIBS=driverLibs)
else:
	PacketizedOpenCL = env.StaticLibrary(target='lib/PacketizedOpenCL', source=driverSrc, CPPDEFINES=llvm_vars.get('CPPDEFINES'), LIBS=driverLibs)


# build AMD-ATI SDKUtil as a static library (required for test applications)
SDKUtil = env.StaticLibrary(target='lib/SDKUtil', source=Split('test/SDKUtil/SDKApplication.cpp test/SDKUtil/SDKBitMap.cpp test/SDKUtil/SDKCommandArgs.cpp test/SDKUtil/SDKCommon.cpp test/SDKUtil/SDKFile.cpp'))
env.Depends(SDKUtil, PacketizedOpenCL)

###
### test applications ###
###

testApps = env.Split("""
FastWalshTransform
""")

#testApps = env.Split("""
#AmbientOcclusionRenderer
#BinomialOption
#BinomialOptionSimple
#BitonicSort
#BlackScholes
#BlackScholesSimple
#DCT
#DwtHaar1D
#EigenValue
#FastWalshTransform
#FloydWarshall
#Histogram
#Mandelbrot
#MatrixTranspose
#NBody
#NBodySimple
#PrefixSum
#RadixSort
#Reduction
#ScanLargeArrays
#SHA1
#SimpleConvolution
#TestSimple
#TestBarrier
#TestBarrier2
#TestLoopBarrier
#TestLoopBarrier2
#Test2D
#TestLinearAccess
#""")

Execute(Mkdir('build/bin'))
if int(compile_dynamic_lib_driver):
	for a in testApps:
		Execute(Copy('build/bin', 'test/'+a+'/'+a+'_Kernels.cl'))
		Obj = env.SharedObject('build/obj/'+a, env.Glob('test/'+a+'/*.cpp'), LIBS=appLibs)
		# The following will only work as soon as Apple uses ICD etc. (OpenCL 1.1)
		#if isDarwin:
			#App = env.Program('build/bin/'+a, env.Glob('test/'+a+'/*.cpp'), LIBS=appLibs, LINKFLAGS=env['LINKFLAGS']+['-framework', 'OpenCL'])
		#else:
			#App = env.Program('build/bin/'+a, env.Glob('test/'+a+'/*.cpp'), LIBS=appLibs)
		App = env.Program('build/bin/'+a, env.Glob('test/'+a+'/*.cpp'), LIBS=appLibs)
		env.Depends(App, Obj)
		env.Depends(App, SDKUtil)
else:
	for a in testApps:
		Execute(Copy('build/bin', 'test/'+a+'/'+a+'_Kernels.cl'))
		Obj = env.StaticObject('build/obj/'+a, env.Glob('test/'+a+'/*.cpp'), LIBS=appLibs+driverLibs)
		App = env.Program('build/bin/'+a, env.Glob('test/'+a+'/*.cpp'), LIBS=appLibs+driverLibs)
		env.Depends(App, Obj)
		env.Depends(App, SDKUtil)


###
### build test applications against AMD OpenCL driver ###
### TODO: We should actually use the OpenCL ICD to chose the driver (and thus only compile it once),
###       but that would require changing the SDKUtil parameter handling, plus we do not provide
###       support for the ICD from packetizedOpenCL side.
###

#env2 = env.Clone()
#env2.Append(CXXFLAGS = env.Split('-DTESTBENCH_BUILD_FROM_CL -DUSE_OPENCL_DRIVER_AMD'))
#env2.Append(LIBPATH = [os.path.join(env['ENV']['AMDAPPSDKROOT'], 'lib/x86_64')])
#appLibs2 = env2.Split('OpenCL SDKUtil glut32 glew32')
#Execute(Mkdir('build/bin/AMD'))
#for a in testApps:
	#Execute(Copy('build/bin/AMD', 'test/'+a+'/'+a+'_Kernels.cl'))
	#Obj = env2.StaticObject('build/obj/AMD/'+a, env2.Glob('test/'+a+'/*.cpp'), LIBS=appLibs2)
	#App = env2.Program('build/bin/AMD/'+a, env2.Glob('build/obj/AMD/'+a+'.obj'), LIBS=appLibs2)
	#env2.Depends(App, Obj)
	#env2.Depends(App, SDKUtil)

###
### build bitcode from OpenCL files ###
###

# NOTE: using --march automatically generates a stub-function
# NOTE: using --march also inserts calls to builtin functions instead of e.g. printf
# NOTE: --march=x86-64 generates bad code for packetization :(
#       --march=x86 (or left out) generates 32bit data structures etc., making wrapper unusable

#env.Command('build/obj/simpleTest_Kernels.ll', 'test/simpleTest_Kernels.cl', "clc --march=x86-64 --msse2 $SOURCE")
#env.Command('build/obj/simpleTest_Kernels.ll', 'test/simpleTest_Kernels.cl', "clc --march=x86 --msse2 $SOURCE")

#cmd_ll = "clc -o $TARGET --msse2 $SOURCE"
#cmd_bc = "llvm-as $SOURCE -o $TARGET"

#for a in testApps:
	#env.Command('build/obj/'+a+'_Kernels', 'test/'+a+'/'+a+'_Kernels.cl',
				#["clc -o ${TARGET.file}.ll --msse2 ${SOURCE}",
				#"llvm-as -o ${TARGET.file}.bc ${TARGET.file}.ll",
				#Delete('${TARGET.file}.ll')])

