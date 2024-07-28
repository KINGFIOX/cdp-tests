VSRC = $(wildcard vsrc/ram.v ../generated/miniRV_SoC.sv)
CSRC = $(wildcard golden_model/*.c) $(wildcard golden_model/stage/*.c) $(wildcard golden_model/peripheral/*.c) $(wildcard csrc/*.c csrc/*.cpp)
SIM_OPTS = --trace -Wno-lint -Wno-style -Wno-TIMESCALEMOD
TEST = lw
TESTFILE = meminit.bin
TRAPFILE = trap_handle.coe
PWD = $(shell pwd)

run_for_python:  # should run "make all" first, for python-based test
	@ln -sf bin/$(TEST).bin $(TESTFILE)
	@./obj_dir/VminiRV_SoC $(TEST)
	@rm -rf $(TESTFILE)

chisel:
	cd .. && sbt 'runMain hitsz.Main'

build: $(VSRC) $(CSRC)
	@verilator -cc --exe --build $(VSRC) --top-module miniRV_SoC $(CSRC) $(SIM_OPTS) +define+PATH=$(TESTFILE) +define+TRAP=$(TRAPFILE) -CFLAGS -DPATH=$(TESTFILE) -ImySoC -CFLAGS -I$(PWD)/golden_model/include
	@mkdir -p waveform

run: build
	@ln -sf bin/$(TEST).bin $(TESTFILE)
	@./obj_dir/VminiRV_SoC $(TEST)
$(TESTFILE):
	ln -sf bin/$(TEST).bin $(TESTFILE)
clean:
	rm -rf obj_dir waveform $(TESTFILE)

wave:
	gtkwave -S load_all_waves.tcl -f waveform/$(TEST).vcd

.PHONY: run debug clean
