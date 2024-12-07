#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

std::unordered_map<uint8_t, std::string> opcodeMap = {
    {0xB8, "mov rax,"}, {0xB9, "mov rcx,"}, {0xBA, "mov rdx,"},
    {0xBB, "mov rbx,"}, {0xBC, "mov rsp,"}, {0xBD, "mov rbp,"},
    {0xBE, "mov rsi,"}, {0xBF, "mov rdi,"}, {0x50, "push rax"},
    {0x51, "push rcx"}, {0x52, "push rdx"}, {0x53, "push rbx"},
    {0x54, "push rsp"}, {0x55, "push rbp"}, {0x56, "push rsi"},
    {0x57, "push rdi"}, {0x58, "pop rax"},  {0x59, "pop rcx"},
    {0x5A, "pop rdx"},  {0x5B, "pop rbx"},  {0x5C, "pop rsp"},
    {0x5D, "pop rbp"},  {0x5E, "pop rsi"},  {0x5F, "pop rdi"},
    {0xE9, "jmp"},      {0xE8, "call"},     {0xC3, "ret"},
    {0x74, "je"},       {0x75, "jne"},      {0x90, "nop"},
    {0xF4, "hlt"},      {0x9B, "wait"},     {0xF0, "cli"},
    {0xF8, "sti"},      {0x0F, "syscall"}
};

std::unordered_map<uint8_t, std::string> registerMap =
{
    {0xC0, "RAX"}, {0xC1, "RCX"}, {0xC2, "RDX"}, {0xC3, "RBX"},
    {0xC4, "RSP"}, {0xC5, "RBP"}, {0xC6, "RSI"}, {0xC7, "RDI"}
};

void disassemble(const std::vector<uint8_t>& bytes)
{
    size_t offset = 0;
    while (offset < bytes.size())
    {
        if (bytes[offset] >= 0x40 && bytes[offset] <= 0x4F)
        {
            offset++;
            continue;
        }

        if (bytes[offset] == 0x00)
        {
            offset++;
            continue;
        }

        uint8_t opcode = bytes[offset];

        if (opcodeMap.find(opcode) != opcodeMap.end())
        {

            if (opcode >= 0xB8 && opcode <= 0xBF)
            {
                std::cout << opcodeMap[opcode];

                uint64_t immediate = 0;
                for (int i = 0; i < 8; ++i)
                {
                    immediate |= static_cast<uint64_t>(bytes[offset + 1 + i]) << (i * 8);
                }
                std::cout << " 0x" << std::hex << std::setw(16) << std::setfill('0') << immediate;
                offset += 9;
            }
            else if (opcode >= 0x50 && opcode <= 0x57)
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode >= 0x58 && opcode <= 0x5F)
            {

                std::cout << opcodeMap[opcode];
                offset += 1;
            }


            else if (opcode == 0xE9)
            {
                std::cout << opcodeMap[opcode];
                int32_t offsetValue = 0;
                for (int i = 0; i < 4; ++i)
                {
                    offsetValue |= static_cast<int32_t>(bytes[offset + 1 + i]) << (i * 8);
                }
                std::cout << " 0x" << std::hex << std::setw(8) << std::setfill('0') << offsetValue;
                offset += 5;
            }
            else if (opcode == 0x74 || opcode == 0x75)
            {
                std::cout << opcodeMap[opcode];

                int8_t offsetValue = static_cast<int8_t>(bytes[offset + 1]);
                std::cout << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(offsetValue);
                offset += 2;
            }
            else if (opcode == 0xE8)
            {
                int32_t displacement = 0;

                for (int i = 0; i < 4; ++i)
                {
                    displacement |= static_cast<int32_t>(bytes[offset + 1 + i]) << (i * 8);
                }

                int32_t address = offset + 5 + displacement;

                std::cout << "call " << "0x" << std::hex << address;

                offset += 5;
            }

            else if (opcode == 0x90)
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode == 0xF4)
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode == 0x9B)
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode == 0xF0) 
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode == 0xF8)
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode == 0xC3)
            {
                std::cout << opcodeMap[opcode];
                offset += 1;
            }
            else if (opcode == 0x0F)
            {
                std::cout << opcodeMap[opcode];
                uint8_t nextByte = bytes[offset + 1];
                if (nextByte == 0x05)
                {
                    offset += 2;
                }
                else
                {
                    offset++;
                }
            }
            else
            {
                offset++;
            }
        }
        else
        {
            std::cout << "[unknown]";
            offset++;
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::vector<uint8_t> binary =
    {
         0xe9, 0x1e, 0x00, 0x00, 0x00,
         0x48, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x48, 0xbf, 0xef, 0xbe, 0xad, 0xde, 0x00, 0x00, 0x00, 0x00,

         0xe9, 0x0c, 0x00, 0x00, 0x00,

         0x48, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x48, 0xbf, 0xef, 0xbe, 0xad, 0xde, 0x00, 0x00, 0x00, 0x00,

         0x75, 0x0c, 0x00, 0x00, 0x00,

         0x48, 0xb8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x48, 0xbf, 0xef, 0xbe, 0xad, 0xde, 0x00, 0x00, 0x00, 0x00,

         0x48, 0xb8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x48, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0xC3
    };
    disassemble(binary);
}