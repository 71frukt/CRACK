.model tiny
.386

.code

org 100h


PSWD_MAX_LEN    equ 10d
ENTER_ASCII     equ 0Dh

ENTER_ASCII     equ 0Dh
ARGS_END_ASCII  equ 24h     ; '$' ascii code


;-------------CODE START-----------------------------------------------------------------------
Start:

    ; welcome
        lea dx, WELCOME_STR
        mov ah, 09h
        int 21h

    ; get pswd
        lea di, PSWD_BUFFER
        call GetPswdToBuffer

        call GetPswdHash        ; ax = hash
        cmp ax, CORRECT_PSWD_HASH
        
        jne incorrect_pswd
        lea dx, CORRECT_PSWD_STR
        mov ah, 09h
        int 21h
        jmp exit

    incorrect_pswd:
        lea dx, INCORRECT_PSWD_STR
        mov ah, 09h
        int 21h

    exit:
        mov ax, 4c00h
		int 21h

;-------------------------------------------------------------------------------------
; Gets Password To Buffer
; Entry:    ds:di = buffer
; Exit:     ds:di = a buffer filled with a password at the end
; Destr:    ax
;-------------------------------------------------------------------------------------
GetPswdToBuffer     proc
        push di

        mov ah, 01h
        
    get_next_pswd_sym:
        int 21h             ; al = received char

        mov byte ptr ds:[di], al
        inc di

        cmp al, ENTER_ASCII
        jne get_next_pswd_sym
    
        pop di

        ret
        endp
;-------------------------------------------------------------------------------------
    


;-------------------------------------------------------------------------------------
; Gets hash code of the specified string
; Entry:    ds:di = string terminating on ENTER_ASCII
; Exit:     ax    = hash code
; Destr:    bx, di
;-------------------------------------------------------------------------------------
GetPswdHash     proc
        mov ax, 5381d   ; start val of hash
    
    next_char_to_hash:
        
        ; hash = hash * 32 + ds:[di++]
        mov bx, 32
        mul bx
        
        mov bl, byte ptr ds:[di]
        add ax, bx
        inc di

        cmp bx, ARGS_END_ASCII
        jne next_char_to_hash

        ret
        endp
;-------------------------------------------------------------------------------------
    

;--------------CODE END----------------------------------------------------------------------




;--------------DATA START-----------------------------------------------------------------------
.data

WELCOME_STR         db 'Hello! Enter the password for authentication: $'

CORRECT_PSWD_STR    db 'CORRECT! Come in!$'
INCORRECT_PSWD_STR  db 'COME OUT!$'

PSWD_BUFFER         db PSWD_MAX_LEN dup(0)
CORRECT_PSWD_HASH   dw 4524h   ; hash of 'ARS LOH'

;--------------DATA END-----------------------------------------------------------------------

end Start