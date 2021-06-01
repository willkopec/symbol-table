/*symtable.h*/

// 
// << William Kopec >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
//
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.
//

#pragma once

#include <iostream>
#include <deque>
#include <map>

using namespace std;

template<typename KeyT, typename SymbolT>
class symtable
{
public:
  //
  // A symbol table is a stack of scopes.  Every scope has a name, and 
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  class Scope
  {
  public:
    string              Name;
    map<KeyT, SymbolT>  Symbols;

    // constructors:
    Scope()
    {
      // default empty string and an empty map:
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
    }
  };

private:
  int Size = 0; //Declare size to 0 by default
  deque <Scope> deck; //Declare the deque under the name deck

public:
  enum class ScopeOption
  {
    ALL, 
    CURRENT,
    GLOBAL
  };

  //
  // default constructor:
  //
  // Creates a new, empty symbol table.  No scope is open.
  //
  symtable()
  {
    //
    // TODO: note that member variables will have their default constructor 
    // called automatically, so there may be nothing to do here.
    //
  }

  //
  // size
  //
  // Returns total # of symbols in the symbol table.
  //
  // Complexity: O(1)
  //
  int size() const
  {
    return Size; //returns the size of the current sym table
  }

  //
  // numscopes
  //
  // Returns the # of open scopes.
  //
  // Complexity: O(1)
  //
  int numscopes() const
  {
    return deck.size(); //returns the number of scopes
  }

  //
  // enterScope
  //
  // Enters a new, open scope in the symbol table, effectively "pushing" on
  // a new scope.  You must provide a name for the new scope, although
  // the name is currently used only for debugging purposes.
  //
  // NOTE: the first scope you enter is known as the GLOBAL scope, since this
  // is typically where GLOBAL symbols are stored.  
  //
  // Complexity: O(1)
  //
  void enterScope(string name)
  {
    Scope thisScope; //declare new scope
    thisScope.Name = name; //set scope name
    deck.push_front(thisScope); //push elements to the front
  }

  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
  //
  void exitScope()
  {
  
    if(deck.empty()){ //if the deck is empty, there is no scope open
        throw runtime_error("symtable::exitScape: empty"); //throw error
    } else {
    Size = (Size - deck.front().Symbols.size()); //else pop front and subtract size from the current scope size to give proper size #
    deck.pop_front();
    }
    
  }

  //
  // curScope
  //
  // Returns a copy of the current scope.  A runtime_error is thrown if
  // no scope is currently open.
  //
  // Complexity: O(N) where N is the # of symbols in the current scope
  //
  Scope curScope() const
  {
    if(deck.empty()){ //if no scope is open give error
        throw runtime_error("symtable::exitScape: empty");
    }
    return deck.front(); //returns the current scope
  }

  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  void insert(KeyT key, SymbolT symbol)
  {
    if(deck.empty()){ //if no scope open
        throw runtime_error("symtable::exitScape: empty"); //give error
    } else {
        if(deck.front().Symbols.count(key) == 1){ //if the key already exists and is just being overwritten, just change the symbol
            deck.front().Symbols[key] = symbol;
        } else { //else, key does not exist, increment size accordingly
			Size++;
            deck.front().Symbols[key] = symbol;
        }
    }
  }

  //
  // lookup
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
  // pair is found, true is returned along with a copy of the symbol (via 
  // "symbol" reference parameter).  If not found, false is returned and
  // the "symbol" parameter is left unchanged.
  //
  // NOTE: the search can be controlled by the "option" parameter.  By 
  // default, the entire symbol table is searched as described above.
  // However, the search can also be limited to just the current scope, 
  // or just the GLOBAL scope, via the "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   string                   symbol;
  //   bool                     found;
  //   ...
  //   found = table.lookup("i", 
  //                        symbol, 
  //                        symtable<string,string>::ScopeOption::CURRENT);
  //
  // Complexity: O(SlgN) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  bool lookup(KeyT key, 
              SymbolT& symbol, 
              ScopeOption option = ScopeOption::ALL) const
  {
    if(deck.empty()){ //if deck is empty, nothing to lookup
        throw runtime_error("symtable::exitScope: empty"); //throw error
    }
    if(option == ScopeOption::CURRENT){ //if the scope option is current
        if(deck.front().Symbols.count(key) == 1){ //check if key exists in the front
            symbol = deck.front().Symbols.at(key); //if it does, set symbol to that key and return true
            return true;
        }
    }
    if(option == ScopeOption::GLOBAL){ //if the scope option is global
        if(deck.back().Symbols.count(key) == 1){ //check if key exists in the front
            symbol = deck.back().Symbols.at(key); //if it does, set symbol to that key and return true
            return true; 
        }
    } 
    if(option == ScopeOption::ALL){ //if the scope option is all
        for(auto& i : deck){
            if(i.Symbols.count(key) == 1){ //check if key exists in the front
                symbol = i.Symbols.at(key); //if it does, set symbol to that key and return true
                return true;
            }
        }  
    }
    
    return false; //if it has finished going through all if statements and hasnt been declared true yet, it is false
  }

  //
  // dump
  // 
  // Dumps the contents of the symbol table to the output stream, 
  // starting with the current scope and working "outward" to the GLOBAL
  // scope.  You can dump the entire symbol table (the default), or dump
  // just the current scope or global scope; this is controlled by the 
  // "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   ...
  //   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
  //
  // Complexity: O(S*N) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
  {
    output << "**************************************************" << endl;
    if (option == ScopeOption::ALL){ 
            output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
            output << "** # of scopes: " << this->numscopes() << endl;
            output << "** # of symbols: " << this->size() << endl;
            for (int i = 0; i < deck.size(); i++) {
            //for loop to iterate through the deque
            output << "** " << deck[i].Name << " **" << endl; 
                 for (auto const& pair: deck[i].Symbols){
                 //for each loop that has complexity required
                    output << pair.first << ": "<< pair.second << endl;
            }
        }
   }
        
        else if (option == ScopeOption::CURRENT){
            output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
            output << "** # of scopes: " << this->numscopes() << endl;
            output << "** # of symbols: " << this->size() << endl;
            output << "** " << deck.front().Name << " **" << endl; 
         for (auto const& pair: deck.front().Symbols){
         //for each loop that has complexity required
            output << pair.first << ": "<< pair.second << endl;
        }
   }
    
    else if(option == ScopeOption::GLOBAL){
            output << "*************** SYMBOL TABLE (GBL) ***************" << endl;
            output << "** # of scopes: " << this->numscopes() << endl;
            output << "** # of symbols: " << this->size() << endl;
            output << "** " << deck.back().Name << " **" << endl; 
        for (auto const& pair: deck.back().Symbols){
        //for each loop that has complexity required
            output << pair.first << ": "<< pair.second << endl;
        }
    }
    //
    // output format per scope:
    //
    // ** scopename **
    // key: symbol
    // key: symbol
    // ...
    //
    output << "**************************************************" << endl;
  }

};
