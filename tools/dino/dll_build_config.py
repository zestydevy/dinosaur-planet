from typing import Any, TextIO
import yaml

class DLLBuildConfig:
    def __init__(self, 
                 compile: bool,
                 link_original_rodata: bool,
                 link_original_data: bool,
                 link_original_bss: bool) -> None:
        self.compile = compile
        self.link_original_rodata = link_original_rodata
        self.link_original_data = link_original_data
        self.link_original_bss = link_original_bss

    def save(self, writer: TextIO) -> None:
        yaml.safe_dump({
           "compile": self.compile, 
           "link_original_rodata": self.link_original_rodata, 
           "link_original_data": self.link_original_data, 
           "link_original_bss": self.link_original_bss
        }, writer)
    
    @staticmethod
    def parse(stream: Any):
        config = yaml.safe_load(stream)

        return DLLBuildConfig(
            compile = config.get("compile", True),
            link_original_rodata = config.get("link_original_rodata", False),
            link_original_data = config.get("link_original_data", False),
            link_original_bss = config.get("link_original_bss", False)
        )
