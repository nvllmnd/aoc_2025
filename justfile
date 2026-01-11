
project_name := "AoC 2025"
project_name_full := "{{project_name}}"
build_root := "./bin"

# Build directories
bin_name := "aoc"
debug_dir := build_root + "/debug"
debug_bin := debug_dir + bin_name

release_dir := build_root + "/release" 
release_bin := release_dir + bin_name


# Compiler selection
compiler := "clang"


# Build type specific configurations
debug_flags := "" #-Db_sanitize=address"
release_flags := "-Db_lto=true -Db_lto_threads=4" 

version_major := "0"
version_minor := "1"
version_patch:= `git rev-list --count HEAD`
version := version_major + "." + version_minor + "." + version_patch


# Lists Available Commands
default: 
	echo "{{project_name}}. Version: {{version}}" 
	just --list


# Prints version
version:
	echo "{{version}}"


# Create debug build dir + configure"
setup-debug:
	if [ ! -d "{{debug_dir}}" ]; then \
		echo "Setting up DEBUG build..."; \
		mkdir -p "{{debug_dir}}"; \
		CC="{{compiler}}" meson setup "{{debug_dir}}" "{{debug_flags}}" "--buildtype=debug";  \
	else \
		echo "Debug Build directory already exists!"; \
		exit 0; \
	fi

#Create release build dir + configure"
setup-release:
	if [ ! -d "{{release_dir}}" ]; then \
		echo "Setting up RELASE build..."; \
		mkdir -p "{{release_dir}}"; \
		CC="{{compiler}}" meson setup "{{release_dir}}" "{{release_flags}}" "--buildtype=release"; \ 
	else \
		echo "Release Build directory already exists!"; \
		exit 0; \
	fi

# Reconfigure existing builds
reconfig-debug:
	test -d "{{debug_dir}}" && meson configure "{{debug_dir}}" "{{debug_flags}}" || just setup-debug

# Reconfigure existing builds
reconfig-release:
	test -d "{{release_dir}}" && meson configure "{{release_dir}}" "{{release_flags}}" || just setup-release

#Compile debug build"
build-debug: setup-debug
	meson compile -C "{{debug_dir}}"

#Compile optimized release build
build-release: setup-release
	meson compile -C "{{release_dir}}"

# Shortcuts / Aliases
alias br := build-release
alias bd := build-debug
alias sd := setup-debug
alias sr := setup-release
alias cfgd := reconfig-debug
alias cfg := reconfig-release

test: build-debug
	meson test -C "{{debug_dir}}"

test-release: build-release
	meson test -C "{{release_dir}}"

# Clean both debug and release directories 
clean:
	rm -rf "{{build_root}}"


# Re-build Project in debug. Runs clean then build
rebuild: clean build-debug

# Re-build Project in release. Runs clean then build
rebuild-release: clean build-release


# Install release build to your systems standard directory
install: build-release
	meson install -C "{{release_dir}}"

dev-setup:
	bash dev/setup.sh

aoc-debug:
	"{{debug_dir}}/aoc" 

aoc:
	"{{release_dir}}/aoc" 


alias rd := rebuild
alias r := rebuild-release

alias build := build-debug
alias run := aoc-debug 
alias run-repl := aoc-debug
alias run-release := aoc 





