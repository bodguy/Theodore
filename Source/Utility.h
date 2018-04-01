#ifndef Utility_h
#define Utility_h

#define SafeDealloc(p) { if(p) { delete (p); (p)=NULL; } }
#define SafeArrayDealloc(p) { if(p) { delete[] (p); (p)=NULL; } }
#define SafeContDealloc(p) { for(auto i : p) SafeDealloc(i); p.clear(); }

#endif /* Utility_h */
