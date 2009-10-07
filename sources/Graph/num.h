/**
 * @file: num.h 
 * Interface and implementation of numeration functionality.
 *
 * @defgroup Nums Numeration of objects
 *
 * @ingroup Graph
 * Numerations can be used to assign numbers to objects.
 * Usage model is following. To make an object numerable you derive it from 'Numbered' class.
 * Then if you have some class representing a container for this objects you derive it from
 * the 'NumManager' class. Altarnatively you can create an instance of NumManager directly.
 * When you want to numerate objects derived from 'Numbered' you ask NumManager ( or derived)
 * for new numeration using NumManager::newNum(). Then use Numbered::setNumber and 
 * Numbered::number to number objects and retrieve object number.
 * When you done with using numbers free the numeration by calling NumManager::freeNum().
 * You can have not more than MAX_NUMERATIONS ( usually 10) at one time so if you forget to 
 * free a numeration you'll run into exception telling you that there are no free numeration
 * indexes left.
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
typedef unsigned short int NumIndex;
typedef unsigned int NumValue;

/**
 * Possible num errors
 *
 * @ingroup Nums
 */
typedef enum NumErrorType_e
{
    /** Some error occured */
    NUM_ERROR_GENERIC,
    /** We've ran out of indexes. Someone forgot to free nums? */
    NUM_ERROR_OUT_OF_INDEXES,
    /** We're out of values. Seems to be interanl error of num class.*/
    NUM_ERROR_OUT_OF_VALUES,
    /** Number is too big */
    NUM_ERROR_NUMBER_OUT_OF_RANGE,
    /** Number of error types */
    NUM_ERROR_NUM
} NumErrorType;

/**
 * Numeration description
 *
 * @ingroup Nums
 */
class Numeration
{
    /** Nums index */
    NumIndex index;
    /** Value */
    NumValue value;

    /** Two classes have acces to num internals. All others do not. */
    friend class Numbered;
    friend class NumManager;
};

/**
 * Num-related constants
 *
 * @ingroup Nums
 */
/** How many numerations are allowed simultaneously */
const short int MAX_NUMERATIONS = 10;
/** Clean value */
const NumValue NUM_VAL_CLEAN = 0;
/** First value */
const NumValue NUM_VAL_FIRST = 1;
/** Last value */
const NumValue NUM_VAL_LAST = ( NumValue)( (int)-1);
/** Value that means object is unnumbered */
const GraphNum NUMBER_NO_NUM = ( GraphNum) -1;
/** Maximal number of object */
const GraphNum NUMBER_MAX = (( GraphNum) -1) - 1;

/**
 * Represents an object that can be involved in numeration
 *
 * @ingroup Nums
 */
class Numbered
{
    /** Numeration descriptions */
    NumValue nums[ MAX_NUMERATIONS];

    /** Number in each numeration */
    GraphNum numbers[ MAX_NUMERATIONS];
public:
    
    Numbered()
    {
        NumIndex i;

        /** Initialize nums */
        for ( i = 0; i < MAX_NUMERATIONS; i++)
        {
            nums [ i] = NUM_VAL_CLEAN;
        }
    }

    /**
     * mark node with num. Return false if node is already numbered. True otherwise.
     */
    inline bool setNumber( Numeration num,
                           GraphNum new_number)
    {
        if ( new_number >= NUMBER_MAX)
            throw NUM_ERROR_NUMBER_OUT_OF_RANGE;

        nums[ num.index] = num.value;
        numbers[ num.index] = new_number;
        if ( nums[ num.index] == num.value)
        {
            return false;
        }
        return true;
    }
    
    /**
     * Return number in given numeration or NO_NUM if it was not numbered yet
     */
    inline GraphNum number( Numeration num)
    {
        if ( nums[ num.index] == num.value)
        {
            return numbers[ num.index];
        }
        return NUMBER_NO_NUM;
    }

    /**
     * Return true if node is numbered in this numeration
     */
    inline bool isNumbered( Numeration num)
    {
        if ( nums[ num.index] == num.value)
        {
            return true;
        }
        return false;
    }

    /**
     * Return true if node has been numbered in this numeration and unmarks it
     */
    inline bool unNumber( Numeration num)
    {
        if ( nums[ num.index] == num.value)
        {
            nums[ num.index] = NUM_VAL_CLEAN;
            return true;
        }
        return false;
    }
        
    /**
     * Clears value for given index
     */
    inline void clear( NumIndex i)
    {
           nums[ i] = NUM_VAL_CLEAN;
    }
};


/**
 * Class that creates/frees nums
 *
 * @ingroup Nums
 */
class NumManager
{
    /**
     * Nums
     */
    NumValue nums[ MAX_NUMERATIONS];
    bool is_used[ MAX_NUMERATIONS];
    NumValue last;

    /** Implementatinal routines */
    /**
     * Find free index
     */
    inline NumIndex findFreeIndex()
    {
        NumIndex i = 0;
        /** Search for free num index */
        for ( i = 0; i < MAX_NUMERATIONS; i++)
        {
            if ( !is_used [ i])
            {
                return i;
            }
        }
        throw NUM_ERROR_OUT_OF_INDEXES;
        return i;
    }
    
    /**
     * Increment num value
     */
    inline NumValue nextValue()
    {
        if ( last == NUM_VAL_LAST)
        {
            last = NUM_VAL_FIRST;
        } else
        {
            last++;
        }
        return last;
    }
 
    /**
     * Check if this value is busy
     */
    inline bool isValueBusy( NumValue val)
    {
        /** Check all nums */
        for ( NumIndex i = 0; i < MAX_NUMERATIONS; i++)
        {
            if ( is_used [ i] && nums[ i] == val)
                return true;
        }
        return false;
    }

    /**
     * MUST BE implemented in inhereted class 
     */
    virtual void clearNumerationsInObjects() = 0;

    /**
     * Return next free value
     */
    inline NumValue findNextFreeValue()
    {
        NumIndex i = 0;
        bool reached_limit = false;
        NumValue res = last;
        
        while( isValueBusy( res))
        {
            /** 
             * If we reached checked NUM_VAL_LAST twice,
             * then we are in infinite loop because for 
             * some reason we don't free our numerations
             */
            if ( res == NUM_VAL_LAST)
            {
                assert< NumErrorType> ( !reached_limit, 
                                           NUM_ERROR_OUT_OF_VALUES);
                clearNumerationsInObjects();
                reached_limit = true;            
            }
            res = nextValue();
        }
        return res;
    }
protected:
    /**
     * Clears unused markers in given object
     */
    inline void clearUnusedNumerations( Numbered *n_obj)
    {
        for ( NumIndex i = 0; i < MAX_NUMERATIONS; i++)
        {
            if ( !is_used [ i])
                n_obj->clear( i);
        }
    }
public:

    /**
     * Default Constructor
     */
    NumManager()
    {
        NumIndex i;

        /** Initialize nums */
        for ( i = 0; i < MAX_NUMERATIONS; i++)
        {
            nums [ i] = NUM_VAL_CLEAN;
            is_used [ i] = false;
        }
        last = NUM_VAL_FIRST;
    }
    
    /**
     * Acquire new num. Nums MUST be freed after use,
     * otherwise you run to numerations number limit.
     */
    Numeration newNum()
    {
        Numeration new_num;
            
        new_num.index = findFreeIndex();
        is_used[ new_num.index] = true;
        new_num.value = findNextFreeValue();
        nums[ new_num.index] = new_num.value;
        return new_num;
    }

    /**
     * Free num
     */
    inline void freeNum( Numeration n)
    {
        is_used[ n.index] = false;
    }
};

