
NAMES  = phschoen
NAMES += Sina
NAMES += Malaika
NAMES += Pauline
NAMES += Mara
NAMES += Marleen


BUILDDIR := ./build
REMOTES_WITH_NAME := $(NAMES:%=$(BUILDDIR)/test_%.stl)

.PHONY: all
all:

define scad_to_stl
	$(1:%.scad=$(BUILDDIR)/%.stl)
endef

#builddir
all: $(BUILDDIR)
$(BUILDDIR):
	mkdir $@

$(BUILDDIR)/%: ./build

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

define build_stl

all:$1
$1: $(BUILDDIR)/$1$(patsubst %,_%, $3).stl

$(BUILDDIR)/$1$(patsubst %,_%, $3).stl : $1.scad
	@echo "Generate $1"
	@$(patsubst %,echo "    with %";, $2)
	openscad -o $$@ $$<  -D "$2"
endef

 #####################################
# actual targest

$(eval $(call build_stl ,quad_charge_plug))

$(eval $(call build_stl ,stick,is_ps2_stick=0))
$(eval $(call build_stl ,stick,is_ps2_stick=1))

$(foreach a,$(NAMES), $(eval $(call build_stl ,quad, name=\"$a\",$a)))

REMOTE_REST=enable_text_engrave=0; show_switch=0; show_lcd=0; show_sticks=0; show_stm32=0; show_antenna=0; show_cc2500=0; show_batery_charger=0; show_batery=0; show_joysticks_pcb=0; show_usb_connector=0;
REMOTE_TOP :=show_top=1; show_bottom=0; show_strapholder=0; show_grip=0; $(REMOTE_REST)
REMOTE_BOTTOM:=show_top=0; show_bottom=1; show_strapholder=0; show_grip=0; $(REMOTE_REST)
REMOTE_STRAP:=show_top=0; show_bottom=0; show_strapholder=1; show_grip=0; $(REMOTE_REST)
REMOTE_GRIP:=show_top=0; show_bottom=0; show_strapholder=0; show_grip=1; $(REMOTE_REST)
$(eval $(call build_stl ,remote,$(REMOTE_TOP),top ))
$(eval $(call build_stl ,remote,$(REMOTE_BOTTOM),bottom))
$(foreach a,$(NAMES), $(eval $(call build_stl ,remote,$(REMOTE_STRAP) name=\"$a\",strap_$a)))
$(eval $(call build_stl ,remote,$(REMOTE_GRIP),grip))
