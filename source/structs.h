#ifndef STRUCTS_H
#define STRUCTS_H
#include <array>

struct DivaInputState
{
    static constexpr int MaxButtonBit = 0x6F;

    enum InputType
    {
        Type_Tapped,
        Type_Released,
        Type_Down,
        Type_DoubleTapped,
        Type_IntervalTapped,
        Type_Max,
    };

    struct ButtonState
    {
        std::array<uint32_t, 4> State;
    };

    ButtonState Tapped;
    ButtonState Released;

    ButtonState Down;
    uint32_t Padding_20[4];

    ButtonState DoubleTapped;
    uint32_t Padding_30[4];

    ButtonState IntervalTapped;

    uint32_t Padding_38[12];
    uint32_t Padding_MM[8];

    int32_t MouseX;
    int32_t MouseY;
    int32_t MouseDeltaX;
    int32_t MouseDeltaY;

    uint32_t Padding_AC[4];

    float LeftJoystickX;
    float LeftJoystickY;
    float RightJoystickX;
    float RightJoystickY;

    char KeyPadding[3];
    char Key;

    uint8_t *GetInputBuffer(InputType inputType)
    {
        switch (inputType)
        {
        case Type_Tapped:
            return reinterpret_cast<uint8_t *>(&Tapped);
        case Type_Released:
            return reinterpret_cast<uint8_t *>(&Released);
        case Type_Down:
            return reinterpret_cast<uint8_t *>(&Down);
        case Type_DoubleTapped:
            return reinterpret_cast<uint8_t *>(&DoubleTapped);
        case Type_IntervalTapped:
            return reinterpret_cast<uint8_t *>(&IntervalTapped);
        default:
            return nullptr;
        }
    }

    void SetBit(uint32_t bit, bool value, InputType inputType)
    {
        uint8_t *data = GetInputBuffer(inputType);

        if (data == nullptr || bit >= MaxButtonBit)
            return;

        int byteIndex = (bit / 8);
        int bitIndex = (bit % 8);

        const uint8_t mask = (1 << bitIndex);
        data[byteIndex] = value ? (data[byteIndex] | mask) : (data[byteIndex] & ~mask);
    }
};

#endif // STRUCTS_H
