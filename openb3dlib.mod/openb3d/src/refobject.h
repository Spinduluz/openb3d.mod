#ifndef REFOBJECT_H
#define REFOBJECT_H

class ReferencedObject {
public:
	ReferencedObject():count(1){
	}

	virtual ~ReferencedObject(){
	}

	int DestroyRef() const{
		int c=--count;
		if(!c) delete this;
		return c;
	}

	void AddRef() const{
		++count;
	}

	int GetRefCount() const{
		return count;
	}

private:
	mutable int count;
};

#endif // REFOBJEC_H

