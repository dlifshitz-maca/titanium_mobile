#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Appcelerator Titanium Mobile
# Copyright (c) 2012 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License
# Please see the LICENSE included with this distribution for details.
#
# Blackberry Application Script
#

import os, sys, shutil
from blackberryndk import BlackberryNDK
from optparse import OptionParser

template_dir = os.path.abspath(os.path.dirname(sys._getframe(0).f_code.co_filename))
top_support_dir = os.path.dirname(template_dir)
sys.path.append(os.path.join(top_support_dir, 'common'))

from mako.template import Template

class Blackberry(object):

	def __init__(self, name, appid, bbndk):
		self.name = name
		self.id = appid
		self.ndk = bbndk

		# TODO MAC: the bar-descriptor likely needs to be regenerated at
		# 	build time to capture changes to tiapp.xml
		# Configuration for the bar-descriptor.xml file
		self.configDescriptor = {
			'id':self.id,
			'appname':self.name,
			'platformversion':self.ndk.version,
			'description':None,
			'version':'1.0',
			'author':'Appcelerator Titanium Mobile', # TODO MAC: Find out how validate the author
			'category':'core.games',                 # TODO MAC: Find out how validate the category
			'icon':'assets/appicon.png',
			'splashscreen':'assets/default.png'
		}

		# Configuration for the project file
		self.configProject = {
			'appname':self.name,
			'buildlocation':None # TODO MAC: Find out how specify the build location
		}

	def create(self, dir):
		project_dir = os.path.join(dir, self.name)
		# Creates directory named as project name.
		# mkbuild utility uses path's last component as project name. So, project directory should be named as project
		build_dir = os.path.join(project_dir, 'build', 'blackberry', self.name)

		if not os.path.exists(build_dir):
			os.makedirs(build_dir)

		# TODO Mac: figure out if we need to do something with version in this script
		#version = os.path.basename(os.path.abspath(os.path.join(template_dir, '..')))

		blackberry_project_resources = os.path.join(project_dir,'Resources','blackberry')
		if os.path.exists(blackberry_project_resources):
			shutil.rmtree(blackberry_project_resources)
		shutil.copytree(os.path.join(template_dir,'resources'),blackberry_project_resources)

		# Copy the tibbapp sample project
		sourcePath = os.path.join(template_dir,'tibbapp')
		for file in os.listdir(sourcePath):
			path = os.path.join(sourcePath, file)
			try:
				if os.path.isdir(path):
					dstDir = os.path.join(build_dir, file)
					if (os.path.exists(dstDir)):
						shutil.rmtree(dstDir)
					shutil.copytree(path, dstDir)
				else:
					shutil.copy2(path, build_dir)
			except Exception, e:
				print >> sys.stderr, e
				sys.exit(1)

		# add replaced templates: bar-descriptor.xml, .project files
		templates = os.path.join(template_dir,'templates')
		# copy bar-descriptor.xml
		shutil.copy2(os.path.join(templates,'bar-descriptor.xml'), build_dir)
		_renderTemplate(os.path.join(build_dir,'bar-descriptor.xml'), self.configDescriptor)
		# copy project file
		shutil.copy2(os.path.join(templates,'project'), os.path.join(build_dir, '.project'))
		_renderTemplate(os.path.join(build_dir,'.project'), self.configProject)

		# import project into workspace so it can be built with mkbuild
		self.ndk.importProject(build_dir)

		# TODO Mac: not sure what this is trying to accomplish
		# create the blackberry resources
		#blackberry_resources_dir = os.path.join(build_dir,'Resources')
		#if not os.path.exists(blackberry_resources_dir):
		#	os.makedirs(blackberry_resources_dir)

def _renderTemplate(template, config):
	tmpl = _loadTemplate(template)
	f = None
	try:
		f = open(template, "w")
		f.write(tmpl.render(config = config))
	except Exception, e:
		print >>sys.stderr, e
		sys.exit(1)
	finally:
		if f != None:
			f.close

def _loadTemplate(template):
	return Template(filename=template, output_encoding='utf-8', encoding_errors='replace')

def __runTemplatingDescriptorTest(configDesc):
	barFile = os.path.join(template_dir, 'templates', 'bar-descriptor.xml')
	try:
		tmpl = _loadTemplate(barFile)
		tmpl.render(config = configDesc)
		return True
	except:
		# Uncomment the following function for debugging
		# __unitTestTraceback()
		return False

def __runTemplatingProjectTest(configProj):
	projectFile = os.path.join(template_dir, 'templates', 'project')
	try:
		tmpl = _loadTemplate(projectFile)
		tmpl.render(config = configProj)
		return True
	except:
		# Uncomment the following function for debugging
		# __unitTestTraceback()
		return False

def __unitTestTraceback():
	from mako.exceptions import RichTraceback
	traceback = RichTraceback()
	print "\n\n-------------START TRACEBACK-------------\n"
	for (filename, lineno, function, line) in traceback.traceback:
		print "File %s, line %s, in %s" % (filename, lineno, function)
		print line
	print "%s: %s" % (str(traceback.error.__class__.__name__), traceback.error)
	print "\n\n-------------END TRACEBACK---------------\n"

def __runUnitTests():
	from tiunittest import UnitTest
	ndk = options.ndk.decode("utf-8") if options.ndk != None else None

	bbndk = BlackberryNDK(ndk)
	bb = Blackberry('TemplateTest', 'com.macadamian.template', bbndk)

	with UnitTest('Test template replacing on bar-descriptor.xml file..'):
		passed =__runTemplatingDescriptorTest(bb.configDescriptor)
		assert passed

	with UnitTest('Test template replacing on .project file..'):
		passed = __runTemplatingProjectTest(bb.configProject)
		assert passed

	print '\nFinished Running Unit Tests'
	UnitTest.printDetails()

if __name__ == '__main__':
	# This script is only meant to be invoked from project.py
	# Setup script usage using optparse
	parser = OptionParser(usage='--name name --id id --dir dir [--ndk ndk] [-t]', description='Creates blackberry project')
	parser.add_option('--name', help='Blackberry project name', dest='name')
	parser.add_option('--id', help='Blackberry project id', dest='id')
	parser.add_option('--dir', help='Blackberry project directory', dest='dir')
	parser.add_option('--ndk', help='Blackberry NDK path', dest='ndk')
	parser.add_option('-t', '--test', help='run unit tests', dest='test', action='store_true')
	(options, args) = parser.parse_args()

	if options.test:
		__runUnitTests()
		sys.exit(0)
	else:
		if options.name == None or options.id == None or options.dir == None:
			parser.print_usage()
			sys.exit(1)

	try:
		bbndk = BlackberryNDK(options.ndk.decode("utf-8") if options.ndk != None else None)
		bb = Blackberry(options.name.decode("utf-8"), options.id.decode("utf-8"), bbndk)
		bb.create(options.dir.decode("utf-8"))
	except Exception, e:
		print >>sys.stderr, e
		sys.exit(1)
