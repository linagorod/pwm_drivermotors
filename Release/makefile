################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
avr_pwm.lss \

FLASH_IMAGE += \
avr_pwm.hex \

EEPROM_IMAGE += \
avr_pwm.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: avr_pwm.elf secondary-outputs

# Tool invocations
avr_pwm.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,avr_pwm.map -mmcu=atmega328p -o "avr_pwm.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

avr_pwm.lss: avr_pwm.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S avr_pwm.elf  >"avr_pwm.lss"
	@echo 'Finished building: $@'
	@echo ' '

avr_pwm.hex: avr_pwm.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex avr_pwm.elf  "avr_pwm.hex"
	@echo 'Finished building: $@'
	@echo ' '

avr_pwm.eep: avr_pwm.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex avr_pwm.elf  "avr_pwm.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: avr_pwm.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p avr_pwm.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) avr_pwm.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
