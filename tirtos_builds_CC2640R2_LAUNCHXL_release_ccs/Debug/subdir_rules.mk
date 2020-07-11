################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1046842913:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1046842913-inproc

build-1046842913-inproc: ../release.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/ccs1000/xdctools_3_61_00_16_core/xs" --xdcpath="C:/ti/simplelink_cc2640r2_sdk_4_20_00_04/source;C:/ti/simplelink_cc2640r2_sdk_4_20_00_04/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2640R2F -r release -c "C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS" --compileOptions " -DDeviceFamily_CC26X0R2 " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1046842913 ../release.cfg
configPkg/compiler.opt: build-1046842913
configPkg/: build-1046842913


