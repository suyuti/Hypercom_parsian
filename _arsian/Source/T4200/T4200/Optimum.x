/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
		  "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(NU_Main)
SEARCH_DIR("/usr/local/xscale-elf/lib");
/* Do we need any of these for elf?
   __DYNAMIC = 0;    */
PROVIDE(fixup_entry_point = NU_Main) ;
SECTIONS
{
  /* Read-only sections, merged into text segment: */
  . = 0x00000000;
  PROVIDE (Image$$text$$Base = .) ;
  PROVIDE (fixup_module_base = .) ;
  .gnu.version    : { *(.gnu.version) }
  .gnu.version_d  : { *(.gnu.version_d) }
  .gnu.version_r  : { *(.gnu.version_r) }
  .init           :
  {
	KEEP (*(.init))
  } =0
  .plt            : { *(.plt) }
  .text           :
  {
	*(VECTORS)
	*(.text .stub .text.* .gnu.linkonce.t.*)
	/* .gnu.warning sections are handled specially by elf32.em.  */
	*(.gnu.warning)
	*(.glue_7t) *(.glue_7)
	*(TCT_CODE) *(TMT_CODE) *(INT_CODE) *(Heap_Code) *(dllCode)
  }
  .fini           :
  {
	KEEP (*(.fini))
  } =0
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);

  . = ALIGN(16) ;
  PROVIDE (fixup_ro_base = .) ;
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.* .constdata) }
  .rodata1        : { *(.rodata1) }
  .sdata2         : { *(.sdata2 .sdata2.* .gnu.linkonce.s2.*) }
  .sbss2          : { *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*) }
  .note.gnu.arm.ident : { KEEP (*(.note.gnu.arm.ident)) }
  .eh_frame_hdr : { *(.eh_frame_hdr) }
  /* Adjust the address for the data segment.  We want to adjust up to
	 the same address within the page on the next page up.  */
  . = ALIGN(256) + (. & (256 - 1));
  /* Ensure the __preinit_array_start label is properly aligned.  We
	 could instead move the label definition inside the section, but
	 the linker would then create the section even if it turns out to
	 be empty, which isn't pretty.  */
  . = ALIGN(32 / 8);
  PROVIDE (__preinit_array_start = .);
  .preinit_array     : { *(.preinit_array) }
  PROVIDE (__preinit_array_end = .);
  PROVIDE (__init_array_start = .);
  .init_array     : { *(.init_array) }
  PROVIDE (__init_array_end = .);
  PROVIDE (__fini_array_start = .);
  .fini_array     : { *(.fini_array) }
  PROVIDE (__fini_array_end = .);
  PROVIDE (Image$$text$$Limit = .) ;
  PROVIDE (Image$$data$$Base = .) ;
  . = ALIGN(16) ;
  PROVIDE (fixup_rw_base = .) ;
  .data           :
  {
	__data_start = . ;
	*(.data .data.* .gnu.linkonce.d.*)
	*(TCT_DATA) *(INT_DATA)
	SORT(CONSTRUCTORS)
  }
  .data1          : { *(.data1) }
  .tdata	  : { *(.tdata .tdata.* .gnu.linkonce.td.*) }
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }
  .eh_frame       : { KEEP (*(.eh_frame)) }
  .gcc_except_table   : { *(.gcc_except_table) }
  .ctors          :
  {
	__CTOR_LIST__ = .;
	LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
	*(SORT(.ctors))
	LONG(0)
	__CTOR_END__ = .;
	/* gcc uses crtbegin.o to find the start of
	   the constructors, so we make sure it is
	   first.  Because this is a wildcard, it
	   doesn't matter if the user does not
	   actually link against crtbegin.o; the
	   linker won't look for a file to match a
	   wildcard.  The wildcard also means that it
	   doesn't matter which directory crtbegin.o
	   is in.  */
	/* KEEP (*crtbegin*.o(.ctors)) */
	/* We don't want to include the .ctor section from
	   from the crtend.o file until after the sorted ctors.
	   The .ctor section from the crtend file contains the
	   end of ctors marker and it must be last */
 /*
	KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
	KEEP (*(SORT(.ctors.*)))
	KEEP (*(.ctors))
*/
  }
  .dtors          :
  {
  __DTOR_LIST__ = .;
	LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
	*(SORT(.dtors))
	LONG(0)
	__DTOR_END__ = .;
/*
	KEEP (*crtbegin*.o(.dtors))
	KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
	KEEP (*(SORT(.dtors.*)))
	KEEP (*(.dtors))
*/
  }
  .jcr            : { KEEP (*(.jcr)) }
  .got            : {
	PROVIDE (fixup_got_base = .) ;
	*(.got.plt) *(.got)
	PROVIDE (fixup_got_end = .) ;
	}
  /* We want the small data sections together, so single-instruction offsets
	 can access them all, and initialized data all before uninitialized, so
	 we can shorten the on-disk segment size.  */
  .sdata          :
  {
	*(.sdata .sdata.* .gnu.linkonce.s.*)
  }
  _edata = .;
  PROVIDE (edata = .);
  PROVIDE (Image$$data$$Limit = .);
  PROVIDE (Image$$bss$$ZI$$Base = .);
  . = ALIGN(16) ;
  PROVIDE (fixup_zi_base = .) ;
  __bss_start = .;
  __bss_start__ = .;
  .sbss           :
  {
	PROVIDE (__sbss_start = .);
	PROVIDE (___sbss_start = .);
	*(.dynsbss)
	*(.sbss .sbss.* .gnu.linkonce.sb.*)
	*(.scommon)
	PROVIDE (__sbss_end = .);
	PROVIDE (___sbss_end = .);
  }
  .bss  :
/*  .bss :*/
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   *(Heap);
   /* Align here to ensure that the .bss section occupies space up to
	  _end.  Align after .bss to ensure correct alignment even if the
	  .bss section disappears because there are no input sections.  */
   . = ALIGN(32 / 8);
  }
  . = ALIGN(32 / 8);
  _end = .;
  _bss_end__ = . ; __bss_end__ = . ; __end__ = . ;
  PROVIDE (end = .);
	. = . + HEAPSIZE ;
  PROVIDE (heapend = .);
  PROVIDE (Image$$bss$$ZI$$Limit = .);
  PROVIDE (fixup_mod_size = .);
  /* Dynamic linking information */
  .interp         : { *(.interp) }
  .hash           : { *(.hash) }
  .dynsym         : { *(.dynsym) }
  .dynstr         : { *(.dynstr) }
  .rel.init       : { *(.rel.init) }
  .rela.init      : { *(.rela.init) }
  /* framos : Include the gcc exception table in a relocatable section */
  .rel.text       : 
    { 
	*(.rel.text .rel.text.* .rel.gnu.linkonce.t.*) 
	*(.rel.gcc_except_table)
    }
  .rela.text      : { *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*) }
  .rel.fini       : { *(.rel.fini) }
  .rela.fini      : { *(.rela.fini) }
  .rel.rodata     : { *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*) }
  .rela.rodata    : { *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*) }
  .rel.data       : { *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*) }
  .rela.data      : { *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*) }
  .rel.tdata	  : { *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*) }
  .rela.tdata	  : { *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*) }
  .rel.tbss	  : { *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*) }
  .rela.tbss	  : { *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*) }
  .rel.ctors      : { *(.rel.ctors) }
  .rela.ctors     : { *(.rela.ctors) }
  .rel.dtors      : { *(.rel.dtors) }
  .rela.dtors     : { *(.rela.dtors) }
  .rel.got        : { *(.rel.got) }
  .rela.got       : { *(.rela.got) }
  .rel.sdata      : { *(.rel.sdata .rel.sdata.* .rel.gnu.linkonce.s.*) }
  .rela.sdata     : { *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*) }
  .rel.sbss       : { *(.rel.sbss .rel.sbss.* .rel.gnu.linkonce.sb.*) }
  .rela.sbss      : { *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*) }
  .rel.sdata2     : { *(.rel.sdata2 .rel.sdata2.* .rel.gnu.linkonce.s2.*) }
  .rela.sdata2    : { *(.rela.sdata2 .rela.sdata2.* .rela.gnu.linkonce.s2.*) }
  .rel.sbss2      : { *(.rel.sbss2 .rel.sbss2.* .rel.gnu.linkonce.sb2.*) }
  .rela.sbss2     : { *(.rela.sbss2 .rela.sbss2.* .rela.gnu.linkonce.sb2.*) }
  .rel.bss        : { *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*) }
  .rela.bss       : { *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*) }
  .rel.plt        : { *(.rel.plt) }
  .rela.plt       : { *(.rela.plt) }
  .dynamic        : { *(.dynamic) }
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  . = ALIGN(16) ;
}
