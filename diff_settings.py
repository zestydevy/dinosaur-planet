#!/usr/bin/env python3

def apply(config, args):
    config['baseimg'] = 'baserom.z64'
    config['myimg'] = 'build/dino.z64'
    config['mapfile'] = 'build/dino.map'
    config['source_directories'] = ['src', 'include']
    config['make_command'] = ['ninja']
