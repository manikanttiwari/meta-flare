from conan import ConanFile

class MetaFlare(ConanFile):
    name = "meta-flare"
    version = "1.0"
    author = "Manikant Tiwari (manikanttiwari8@gmail.com)"
    generators = ("CMakeDeps", "CMakeToolchain")
    settings = ("os", "build_type", "arch", "compiler")

    def requirements(self):
        self.requires("zookeeper-client-c/3.9.0")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.25]")

    # def configure(self):
    #     pass

    def layout(self):
        self.folders_generators = ""