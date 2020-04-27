# Ce am implementat

- [x] Lambda-NFA -> NFA
- [x] NFA -> DFA
- [x] DFA -> Min-DFA

Mentionez ca fiecare functioneaza corect si functioneaza si daca le folosesc la un loc, adica:

```cpp
fsm::lnfa lnfa{ builder };
fsm::nfa nfa{ lnfa.to_nfa() };
fsm::dfa dfa{ nfa.to_dfa() };
fsm::dfa min_dfa{ dfa.minimize() };
```
functioneaza corect pe exemplele din pdf-uri. Am testat lambda nfa-ul din laboratorul 3-4 cu toate transformarile.

Testele sunt in [tests/conversions.cpp](https://github.com/AlexandruIca/LFA_Tema2/blob/master/tests/conversions.cpp). Acolo afisez si structura automatelor.
