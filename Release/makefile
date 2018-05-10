################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include UART/subdir.mk
-include SH1106/subdir.mk
-include ADC/subdir.mk
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
inzynierka_pcb_final.lss \

FLASH_IMAGE += \
inzynierka_pcb_final.hex \

EEPROM_IMAGE += \
inzynierka_pcb_final.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: inzynierka_pcb_final.elf secondary-outputs

# Tool invocations
inzynierka_pcb_final.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,inzynierka_pcb_final.map -mmcu=atmega32 -o "inzynierka_pcb_final.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

inzynierka_pcb_final.lss: inzynierka_pcb_final.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S inzynierka_pcb_final.elf  >"inzynierka_pcb_final.lss"
	@echo 'Finished building: $@'
	@echo ' '

inzynierka_pcb_final.hex: inzynierka_pcb_final.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex inzynierka_pcb_final.elf  "inzynierka_pcb_final.hex"
	@echo 'Finished building: $@'
	@echo ' '

inzynierka_pcb_final.eep: inzynierka_pcb_final.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex inzynierka_pcb_final.elf  "inzynierka_pcb_final.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: inzynierka_pcb_final.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 inzynierka_pcb_final.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) inzynierka_pcb_final.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
