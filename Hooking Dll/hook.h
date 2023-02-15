#pragma once
#include <Windows.h>

/* INTERNAL FUNCTIONS */

/// <summary>
/// Hooks src function to dest 
/// Len = amount of bytes overwritten
/// </summary>
/// <param name="src"></param>
/// <param name="dest"></param>
/// <param name="len"></param>
void Hook(void* src, void* dest, int len)
{
    /*
    * x86 unconditional jmp is 5 bytes in length
    * if location we insert jmp to is less than 5 bytes
    * cannot insert jmp instruction
    */
    if (len < 5)
        return;

    // saving original page permissions 
    DWORD currentProtection;
    // size property is length (amount of bytes we will be overwriting)
    // can also be the size of the function 
    // makes more sense to use amount of bytes
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &currentProtection);

    // this is setting the bytes at the location specified to nop 
    // this is to ensure there are no "stray" bytes which will interfere with the hook
    // the amount of nop instructions to set is how many bytes will be overwritten
    memset(src, 0x90, len);

    /*
    
    Setting all the existing bytes to nop instructions is crucial to avoid any possible issues
    Now we will be overwriting some of those nops to place our jmp opcode at the function address
    then after our jump we will place our relative offset - the location which we want to jump to

    You could just write the jmp and jmp location without setting 
    but why cause yourself unwanted issues? 
    */

    // as previously stated relative address is the location 
    // where the jmp instruction will be placed
    DWORD relativeAddress = (DWORD)dest - (DWORD)src - 5;

    // at the location we want to hook
    // set the first byte to E9 which is the first part of the jmp instruction
    // the next four bytes will be the relative offset (where to jump to)
    // this makes up a five byte jmp instruction

    /*Casting location of function to a byte ptr and dereferencing to set our opcode for a jmp instruction which is one byte*/
    *(PBYTE)src = 0xE9;
    /*Now casting the function address + 1 to a DWORD representing an address - to set the relative offset for the jmp instruction which is four bytes*/
    *(DWORD*)((DWORD)src + 1) = relativeAddress;

    // restoring original page permissions
    DWORD newProtect;
    VirtualProtect(src, len, currentProtection, &newProtect);
}

/// <summary>
/// Inserts nop instructions at given address based on size as the amount of bytes to write 
/// </summary>
/// <param name="dest"></param>
/// <param name="size"></param>
void Nop(void* dest, int size)
{
    DWORD oldProtect;
    VirtualProtect(dest, 0, PAGE_EXECUTE_READWRITE, &oldProtect);
    memset(dest, 0x90, size);
    DWORD newProtect;
    VirtualProtect(dest, 0, oldProtect, &newProtect);
}
