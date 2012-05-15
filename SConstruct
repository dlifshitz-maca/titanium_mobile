#!/usr/bin/env python
#
# Top level scons script
#
# Usage:
#
# TODO: fill in the rest
# blackberry        1 to build only blackberry, 0 to skip
# blackberry_ndk    path to the BlackBerry NDK
#
import os, shutil, platform, os.path as path, sys
import package
import SCons.Variables
import SCons.Environment
import subprocess
from SCons.Script import *

# read version from the build folder
# this is used by other python scripts too
cwd = os.path.abspath(os.path.dirname(sys._getframe(0).f_code.co_filename))
sys.path.append(path.join(cwd,"build"))
sys.path.append(path.join(cwd,"support","android"))
sys.path.append(path.join(cwd,"support","blackberry"))
sys.path.append(path.join(cwd,"support","common"))
import titanium_version, ant
from tilogger import TiLogger
from androidsdk import AndroidSDK
from blackberryndk import BlackberryNDK
version = titanium_version.version
module_apiversion = titanium_version.module_apiversion

# allow it to be overriden on command line or in env
if os.environ.has_key('PRODUCT_VERSION'):
	version = os.environ['PRODUCT_VERSION']
elif ARGUMENTS.get('PRODUCT_VERSION', 0):
	version = ARGUMENTS.get('PRODUCT_VERSION')

# get the githash for the build so we can always pull this build from a specific
# commit.  We're getting it here so we can pass it to android's ant build
# in order to get it into build.properties
gitCmd = "git"
if platform.system() == "Windows":
	gitCmd += ".exe"

p = subprocess.Popen([gitCmd, "rev-parse", "HEAD"], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
out, err = p.communicate()
if err:
	print >>sys.stderr, err
	print >>sys.stderr, "Error executing git rev-parse, is git in your PATH?"
	sys.exit(1)

githash = out.strip()[:7]
print "Building MobileSDK version %s, githash %s" % (version, githash)

#
# this is messy, but i don't care, scons makes it too
# hard to include python after an external SConscript
#

flags = ''

only_package = False
if ARGUMENTS.get("package",0):
	only_package = True

install = "install" in COMMAND_LINE_TARGETS or ARGUMENTS.get("install", 0)
clean = "clean" in COMMAND_LINE_TARGETS or ARGUMENTS.get("clean", 0)
run_drillbit = "drillbit" in COMMAND_LINE_TARGETS or ARGUMENTS.get("drillbit",0)

if clean and os.path.exists('iphone/iphone/build'):
	shutil.rmtree('iphone/iphone/build')

build_type = 'full'
force_iphone = False
if ARGUMENTS.get('iphone',0):
	build_type='iphone'

if ARGUMENTS.get('android',0):
	build_type='android'

if ARGUMENTS.get('ipad',0):
	build_type='ipad'

if ARGUMENTS.get('mobileweb',0):
	build_type='mobileweb'

if ARGUMENTS.get('blackberry',0):
	build_type='blackberry'

if ARGUMENTS.get('force_iphone',0):
	force_iphone = True

if ARGUMENTS.get('COMPILER_FLAGS', 0):
	flags = ARGUMENTS.get('COMPILER_FLAGS')

env = Environment()
Export("env cwd version")
if build_type in ['full', 'android'] and not only_package:
	d = os.getcwd()
	os.chdir('android')
	try:
		sdk = AndroidSDK(ARGUMENTS.get("android_sdk", None), 8)
		# TODO re-enable javadoc targets = ["full.build", "build.titanium.javadoc"]
		targets = ["full.build"]
		if clean: targets = ["clean"]
		elif "ant_targets" in ARGUMENTS: targets = ARGUMENTS["ant_targets"].split(",")

		javah_path = os.path.join(os.path.dirname(ant.get_java()), "javah")

		ant.build(targets=targets, properties={"build.version": version, "build.githash": githash,
			"android.sdk": sdk.get_android_sdk(), "android.platform": sdk.get_platform_dir(), "google.apis": sdk.get_google_apis_dir(),
			"ndk.build.args": "JAVAH=%s" % javah_path })
	finally:
		os.chdir(d)

if build_type in ['full', 'iphone', 'ipad'] and not only_package \
	and (platform.system() == "Darwin" or force_iphone):
	d = os.getcwd()
	os.chdir('iphone')
	try:
		#output = 0
		iphone_build_type = "clean" if clean else build_type
		output = os.system("scons PRODUCT_VERSION=%s COMPILER_FLAGS='%s' BUILD_TYPE='%s'" % (version,flags,iphone_build_type))	
		if output!=0:
			sys.stderr.write("BUILD FAILED!!!!\n")
			# beep, please
			if platform.system() == "Darwin":
				os.system("say 'OH NO...the build failed!!!'")
				os.system("printf \"\a\"")
				os.system("printf \"\a\"")
				os.system("printf \"\a\"")
			sys.exit(output)
	finally:
		os.chdir(d)

if build_type in ['full', 'mobileweb'] and not only_package:
	d = os.getcwd()
	os.chdir('mobileweb')
	try:
		mobileweb_build_type = "clean" if clean else build_type
		# nothing to do... yet
	finally:
		os.chdir(d)

if build_type in ['full', 'blackberry'] and not only_package:
	d = os.getcwd()
	try:
		os.chdir('blackberry')
		log = TiLogger(os.path.join(cwd, 'scons_blackberry.log'))
		bbndk = BlackberryNDK(ARGUMENTS.get("blackberry_ndk", None), log = log)
		if clean:
			blackberryBuildType = "clean"
			print 'Cleaning for BlackBerry'
		else:
			blackberryBuildType = "all"
			print 'Building for BlackBerry'
		retCode = bbndk.buildTibb(os.path.join(os.getcwd(), 'tibb'), blackberryBuildType)
		if retCode != 0:
			sys.stderr.write("BUILD FAILED!!!!\n")
			sys.exit(retCode)
	finally:
		os.chdir(d)

def install_mobilesdk(version_tag):
	if (platform.system() == "Darwin"):
		os_names = { "Windows":"win32", "Linux":"linux", "Darwin":"osx" }
		os_name = os_names[platform.system()]
		mobilesdk_zipfile = os.path.join(os.path.abspath('dist'), 'mobilesdk-%s-%s.zip' % (version_tag, os_name))
		print "Installing %s..." % mobilesdk_zipfile
		installation_directory = os.path.expanduser('~/Library/Application Support/Titanium')
		os.system('/usr/bin/unzip -q -o -d "%s" "%s"' % (installation_directory, mobilesdk_zipfile))

def package_sdk(target, source, env):
	android = build_type in ['full', 'android']
	iphone = build_type in ['full', 'iphone']
	ipad = build_type in ['full', 'ipad']
	mobileweb = build_type in ['full', 'mobileweb']
	blackberry = build_type in ['full', 'blackberry']
	package_all = ARGUMENTS.get('package_all', 0)
	version_tag = ARGUMENTS.get('version_tag', version)
	build_jsca = int(ARGUMENTS.get('build_jsca', 1))
	print "Packaging MobileSDK (%s)..." % version_tag
	packager = package.Packager(build_jsca=build_jsca)
	if package_all:
		packager.build_all_platforms(os.path.abspath('dist'), version, module_apiversion, android, iphone, ipad, mobileweb, blackberry, version_tag)
	else:
		packager.build(os.path.abspath('dist'), version, module_apiversion, android, iphone, ipad, mobileweb, blackberry, version_tag)
	if install and not clean:
		install_mobilesdk(version_tag)

def drillbit_builder(target, source, env):
	sys.path.append("drillbit")
	import drillbit
	drillbit.build_and_run(android_sdk=sdk.get_android_sdk())

if run_drillbit:
	drillbit = Builder(action = drillbit_builder)
	env.Append(BUILDERS = {'BuildAndRunDrillbit': drillbit})
	env.BuildAndRunDrillbit('#dummy-drillbit-target', [])

package_builder = Builder(action = package_sdk)
env.Append(BUILDERS = {'PackageMobileSDK': package_builder})
env.PackageMobileSDK("#dummy-sdk-target", [])
	
if clean:
	# don't error 
	Exit(0)
