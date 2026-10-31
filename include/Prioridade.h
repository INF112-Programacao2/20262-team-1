#ifndef PRIORIDADE_H
#define PRIORIDADE_H

// Os valores seguem a escala int usada pela FilaAtendimento,
// então static_cast<int>(Prioridade::URGENTE) == 3.
enum class Prioridade {
    NAO_URGENTE   = 1,
    POUCO_URGENTE = 2,
    URGENTE       = 3,
    MUITO_URGENTE = 4,
    EMERGENCIA    = 5
};

constexpr int PRIORIDADE_MINIMA = 1;
constexpr int PRIORIDADE_MAXIMA = 5;

#endif
