; VESA assembly Functions

; Returns 16bit int
[GLOBAL testvesa]
testvesa:
    mov ah, 4Fh
    mov al, 00h
    cli
    int 10h
    sti
    push ax
    ret
