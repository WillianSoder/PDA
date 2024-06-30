# PDA implementation

This project is a Pushdown Automaton (PDA) simulator written in C. It was created using graph theory, where each vertex represents a ‘state’ of the PDA, and each edge corresponds to a ‘transition’. To determine word acceptance, a backtracking algorithm based on Depth-First Search (DFS) was employed. The project enables users to define a PDA by specifying states, transitions, and final states. Users can then input words to verify if they are accepted by the PDA.

# Getting Started

###  Prerequisites
- any C language compiler


## Steps for use the PDA Simulator
- these steps are based on a PDA that recognizes the language:
{a^nb^n | n ∈ N}.

### 1°: Input the number of states and transitions

```bash 
    3
    4
```
### 2°: Input the transitions:
- enter the transitions; 
- format: origin consume pop push destiny;
- the symbol "&" indicates the empty word (ε).

```bash
    0 a & A 0
    0 & & & 1
    1 b A & 1
    1 & Z Z 2
```
### 3°: Define the final states:
- enter Number of final states;
- enter which nodes will be the final states.

```bash
    1 
    2
```
### 4°: Enter words for verification:
 - type the desired word then click “enter”;  
 - type '*' to stop.
```bash
    ab
    aabb
    aabba
    abb
    aab
    aaabbb
    aaaaabbbb
    aaaaabbbbb
    aaaaabbbbbb
    *
```

### 5°: OUTPUT
```bash
    ab: Yes
    aabb: Yes
    aabba: Not
    abb: Not
    aab: Not
    aaabbb: Yes
    aaaaabbbb: Not
    aaaaabbbbb: Yes
    aaaaabbbbbb: Yes
```



## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue if you have any suggestions or improvements.
