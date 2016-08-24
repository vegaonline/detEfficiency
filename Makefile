.PHONY: clean All

All:
	@echo "----------Building project:[ det_Eff - Debug ]----------"
	@"$(MAKE)" -f  "det_Eff.mk"
clean:
	@echo "----------Cleaning project:[ det_Eff - Debug ]----------"
	@"$(MAKE)" -f  "det_Eff.mk" clean
