.PHONY: clean All

All:
	@echo "----------Building project:[ WrapAL - Release ]----------"
	@cd "WrapAL" && "$(MAKE)" -f  "WrapAL.mk"
	@echo "----------Building project:[ demo2 - Release ]----------"
	@cd "demo2" && "$(MAKE)" -f  "demo2.mk"
clean:
	@echo "----------Cleaning project:[ WrapAL - Release ]----------"
	@cd "WrapAL" && "$(MAKE)" -f  "WrapAL.mk"  clean
	@echo "----------Cleaning project:[ demo2 - Release ]----------"
	@cd "demo2" && "$(MAKE)" -f  "demo2.mk" clean
