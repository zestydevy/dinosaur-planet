from typing import Any, TextIO
import yaml

class DLLBuildConfig:
    def __init__(self, 
                 compile: bool) -> None:
        self.compile = compile

    def save(self, writer: TextIO) -> None:
        yaml.safe_dump({
           "compile": self.compile
        }, writer)
    
    @staticmethod
    def parse(stream: Any):
        config = yaml.safe_load(stream)

        return DLLBuildConfig(
            compile = config.get("compile", True)
        )
