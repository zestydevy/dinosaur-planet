#!/usr/bin/env python3

def apply(config, args):
    config['baseimg'] = 'baserom.z64'
    config['myimg'] = 'dino.z64'
    config['mapfile'] = 'build/dino.map'
    config['source_directories'] = ['.']
