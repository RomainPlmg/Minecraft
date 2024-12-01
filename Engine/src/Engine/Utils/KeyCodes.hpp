#pragma once

namespace Engine {

/* From glfw3.h */
enum KeyCode {

    Backspace = 259,
    Tab = 258,
    Enter = 257,
    Alt = 342,
    Pause = 284,
    CapsLock = 280,
    Esc = 256,
    Space = 32,
    PageUp = 266,
    PageDown = 267,
    End = 269,
    Home = 268,
    Leftarrow = 263,
    Uparrow = 265,
    RightArrow = 262,
    DownArrow = 264,
    PrintScreen = 283,
    Insert = 260,
    Delete = 261,

    D0 = 48,
    D1 = 49,
    D2 = 50,
    D3 = 51,
    D4 = 52,
    D5 = 53,
    D6 = 54,
    D7 = 55,
    D8 = 56,
    D9 = 57,

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    DN0 = 320,
    DN1 = 321,
    DN2 = 322,
    DN3 = 323,
    DN4 = 324,
    DN5 = 325,
    DN6 = 326,
    DN7 = 327,
    DN8 = 328,
    DN9 = 329,
    DNMult = 332,
    DNPlus = 332,
    DNMinus = 333,
    DNPoint = 330,
    DNDivide = 331,

    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,

    NumLock = 282,
    ScrollLock = 281,
    LeftShift = 340,
    RightShift = 344,
    LeftCtrl = 341,
    RightCtrl = 345
};

const std::map<KeyCode, std::string> KeyNames{std::make_pair(Backspace, "Backspace"),
                                              std::make_pair(Tab, "Tab"),
                                              std::make_pair(Enter, "Enter"),
                                              std::make_pair(LeftShift, "LeftShift"),
                                              std::make_pair(LeftCtrl, "LeftCtrl"),
                                              std::make_pair(Alt, "Alt"),
                                              std::make_pair(Pause, "Pause"),
                                              std::make_pair(CapsLock, "CapsLock"),
                                              std::make_pair(Esc, "Esc"),
                                              std::make_pair(Space, "Space"),
                                              std::make_pair(PageUp, "PageUp"),
                                              std::make_pair(PageDown, "PageDown"),
                                              std::make_pair(End, "End"),
                                              std::make_pair(Home, "Home"),
                                              std::make_pair(Leftarrow, "Leftarrow"),
                                              std::make_pair(Uparrow, "Uparrow"),
                                              std::make_pair(RightArrow, "RightArrow"),
                                              std::make_pair(DownArrow, "DownArrow"),
                                              std::make_pair(PrintScreen, "PrintScreen"),
                                              std::make_pair(Insert, "Insert"),
                                              std::make_pair(Delete, "Delete"),
                                              std::make_pair(D0, "D0"),
                                              std::make_pair(D1, "D1"),
                                              std::make_pair(D2, "D2"),
                                              std::make_pair(D3, "D3"),
                                              std::make_pair(D4, "D4"),
                                              std::make_pair(D5, "D5"),
                                              std::make_pair(D6, "D6"),
                                              std::make_pair(D7, "D7"),
                                              std::make_pair(D8, "D8"),
                                              std::make_pair(D9, "D9"),
                                              std::make_pair(A, "A"),
                                              std::make_pair(B, "B"),
                                              std::make_pair(C, "C"),
                                              std::make_pair(D, "D"),
                                              std::make_pair(E, "E"),
                                              std::make_pair(F, "F"),
                                              std::make_pair(G, "G"),
                                              std::make_pair(H, "H"),
                                              std::make_pair(I, "I"),
                                              std::make_pair(J, "J"),
                                              std::make_pair(K, "K"),
                                              std::make_pair(L, "L"),
                                              std::make_pair(M, "M"),
                                              std::make_pair(N, "N"),
                                              std::make_pair(O, "O"),
                                              std::make_pair(P, "P"),
                                              std::make_pair(Q, "Q"),
                                              std::make_pair(R, "R"),
                                              std::make_pair(S, "S"),
                                              std::make_pair(T, "T"),
                                              std::make_pair(U, "U"),
                                              std::make_pair(V, "V"),
                                              std::make_pair(W, "W"),
                                              std::make_pair(X, "X"),
                                              std::make_pair(Y, "Y"),
                                              std::make_pair(Z, "Z"),
                                              std::make_pair(DN0, "DN0"),
                                              std::make_pair(DN1, "DN1"),
                                              std::make_pair(DN2, "DN2"),
                                              std::make_pair(DN3, "DN3"),
                                              std::make_pair(DN4, "DN4"),
                                              std::make_pair(DN5, "DN5"),
                                              std::make_pair(DN6, "DN6"),
                                              std::make_pair(DN7, "DN7"),
                                              std::make_pair(DN8, "DN8"),
                                              std::make_pair(DN9, "DN9"),
                                              std::make_pair(DNMult, "DNMult"),
                                              std::make_pair(DNPlus, "DNPlus"),
                                              std::make_pair(DNMinus, "DNMinus"),
                                              std::make_pair(DNPoint, "DNPoint"),
                                              std::make_pair(DNDivide, "DNDivide"),
                                              std::make_pair(F1, "F1"),
                                              std::make_pair(F2, "F2"),
                                              std::make_pair(F3, "F3"),
                                              std::make_pair(F4, "F4"),
                                              std::make_pair(F5, "F5"),
                                              std::make_pair(F6, "F6"),
                                              std::make_pair(F7, "F7"),
                                              std::make_pair(F8, "F8"),
                                              std::make_pair(F9, "F9"),
                                              std::make_pair(F10, "F10"),
                                              std::make_pair(F11, "F11"),
                                              std::make_pair(F12, "F12"),
                                              std::make_pair(NumLock, "NumLock"),
                                              std::make_pair(ScrollLock, "ScrollLock"),
                                              std::make_pair(LeftShift, "LeftShift"),
                                              std::make_pair(RightShift, "RightShift"),
                                              std::make_pair(LeftCtrl, "LeftCtrl"),
                                              std::make_pair(RightCtrl, "RightCtrl")};
}  // namespace Engine