/*
 *  @(#) $Id: shelf.ld 1824 2010-07-02 06:50:03Z mit-kimai $
 */

OUTPUT_FORMAT("elf32-sh","elf32-sh","elf32-sh")
OUTPUT_ARCH(sh)
STARTUP(start.o)

PROVIDE(_hardware_init_hook = 0);
PROVIDE(_software_init_hook = 0);
PROVIDE(_software_term_hook = 0);
SECTIONS
{
    .text :	
    {
        __text = . ;
		*(.vector)
        *(.text)
	
    }
	.vector_entry :
	{
		__vector_entry_start = .;
		*(.vector_entry)

	}
	__vector_entry_end = .;
    _etext = .	;
    PROVIDE (etext = .)	;
    .rodata : { *(.rodata) }
    . = ALIGN(4);
    __idata_start = . ;
    .data   :  AT(__idata_start) 
    {
        __data_start = . ;
        *(.data)
    }
    __idata_end = __idata_start + SIZEOF(.data);
    _edata  =  . ;
    PROVIDE (edata = .);
    . = ALIGN(4);
    __bss_start = .	;		
    .bss       :
    {

      *(.bss)
      *(COMMON)
    }
    _end = .		;
    PROVIDE (end = .)	;
    .comment 0 : { *(.comment) }	
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }	
}	
