/**
 * @file: marker.h 
 * Interface and implementation of marker functionality.
 *
 * @defgroup Mark Markers
 *
 * @ingroup Graph
 *
 * Markers are used to mark objects. An object can be marked with several markers.
 * For each marker we can test if an object is marked with it.
 * To use this functinality you have to subclass Marked class to obtain marked 
 * object and subclass MarkerManager class to create and free markers.
 *
 * When implementing an algorithm with markers you first create it by MarkerManager::newMarker,
 * pass it around in your algorithm marking objects with Marked::mark and testing them with 
 * Marked::isMarked. After your done free marker with MarkerManager::freeMarker
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
/** Marker index type */
typedef unsigned short int MarkerIndex;
/** Marker value type */
typedef unsigned int MarkerValue;

/**
 * Possible marker errors
 *
 * @ingroup Mark
 */
typedef enum MarkerErrorType_e
{
    /** Some error occured */
    M_ERROR_GENERIC,
    /** We've ran out of indexes. Someone forgot to free markers? */
    M_ERROR_OUT_OF_INDEXES,
    /** We're out of values. Seems to be interanl error of marker class.*/
    M_ERROR_OUT_OF_VALUES,
    /** Number of error types */
    M_ERROR_NUM
} MarkerErrorType;

/**
 * Marker description
 *
 * @ingroup Mark
 */
class Marker
{
    /** Markers index */
    MarkerIndex index;
    /** Value */
    MarkerValue value;

    /** Two classes have acces to marker internals. All others do not. */
    friend class Marked;
    friend class MarkerManager;
};

/**
 * Marker-related constants
 */
/** How many markers are allowed simultaneously */
const short int MAX_GRAPH_MARKERS = 10;
/** Clean value of markers */
const MarkerValue GRAPH_MARKER_CLEAN = 0;
/** First value of markers */
const MarkerValue GRAPH_MARKER_FIRST = 1;
/** Last value of markers */
const MarkerValue GRAPH_MARKER_LAST = ( MarkerValue)( (int)-1);

/**
 * Represents a marked object
 *
 * @ingroup Mark
 */
class Marked
{
    /** Markers */
    MarkerValue markers[ MAX_GRAPH_MARKERS];
public:
    
    /**
	 * Default constructor
	 */
	Marked()
    {
        MarkerIndex i;

        /** Initialize markers */
        for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            markers [ i] = GRAPH_MARKER_CLEAN;
        }
    }

    /**
     * mark node with marker. Return false if node is already marked. True otherwise.
     */
    inline bool mark( Marker marker)
    {
        if ( markers[ marker.index] == marker.value)
        {
            return false;
        }
        markers[ marker.index] = marker.value;
        return true;
    }

    /**
     * Return true if node is marked with this marker
     */
    inline bool isMarked( Marker marker)
    {
        if ( markers[ marker.index] == marker.value)
        {
            return true;
        }
        return false;
    }

    /**
     * Return true if node has been marked with this marker and unmarks it
     */
    inline bool unmark( Marker marker)
    {
        if ( markers[ marker.index] == marker.value)
        {
            markers[ marker.index] = GRAPH_MARKER_CLEAN;
            return true;
        }
        return false;
    }
    /**
     * Clears value for given index
     */
    inline void clear( MarkerIndex i)
    {
           markers[ i] = GRAPH_MARKER_CLEAN;
    }
};


/**
 * Class that creates/frees markers
 *
 * @ingroup Mark
 */
class MarkerManager
{
    /**
     * Markers
     */
    MarkerValue markers[ MAX_GRAPH_MARKERS];
    bool is_used[ MAX_GRAPH_MARKERS];
    MarkerValue last;

    /** Implementatinal routines */
    /**
     * Find free index
     */
    inline MarkerIndex findFreeIndex()
    {
        MarkerIndex i = 0;
        /** Search for free marker index */
        for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            if ( !is_used [ i])
            {
                return i;
            }
        }
        throw M_ERROR_OUT_OF_INDEXES;
        return i;
    }
    
    /**
     * Increment marker value
     */
    inline MarkerValue nextValue()
    {
        if ( last == GRAPH_MARKER_LAST)
        {
            last = GRAPH_MARKER_FIRST;
        } else
        {
            last++;
        }
        return last;
    }
 
    /**
     * Clear markers in marked objects, MUST BE implemented in inhereted class 
     */
    virtual void clearMarkersInObjects() = 0;

    /**
     * Check if this value is busy
     */
    inline bool isValueBusy( MarkerValue val)
    {
        /** Check all markers */
        for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            if ( is_used [ i] && markers[ i] == val)
                return true;
        }
        return false;
    }

    /**
     * Return next free value
     */
    inline MarkerValue findNextFreeValue()
    {
        MarkerIndex i = 0;
        bool reached_limit = false;
        MarkerValue res = last;
        
        while( isValueBusy( res))
        {
            /** 
             * If we reached checked GRAPH_MARKER_LAST twice,
             * then we are in infinite loop and for some reason we don't free our markers
             */
            if ( res == GRAPH_MARKER_LAST)
            {
                assert< MarkerErrorType> ( !reached_limit, 
                                           M_ERROR_OUT_OF_VALUES);
                clearMarkersInObjects();
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
    inline void clearUnusedMarkers( Marked *m_obj)
    {
        for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            if ( !is_used [ i])
                m_obj->clear( i);
        }
    }
public:

    /**
     * Default Constructor
     */
    MarkerManager()
    {
        MarkerIndex i;

        /** Initialize markers */
        for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            markers [ i] = GRAPH_MARKER_CLEAN;
            is_used [ i] = false;
        }
        last = GRAPH_MARKER_FIRST;
    }
    
    /**
     * Acquire new marker. Markers MUST be freed after use,
     * otherwise you run to markers number limit.
     */
    Marker newMarker()
    {
        try {
            Marker new_marker;
                
            new_marker.index = findFreeIndex();
            is_used[ new_marker.index] = true;
            new_marker.value = findNextFreeValue();
            markers[ new_marker.index] = new_marker.value;
            return new_marker;
        } catch ( MarkerErrorType type)
        {
            /** Handle errors */
            switch ( type)
            {
                case M_ERROR_GENERIC:    
                default:
                    assert(0);
            }
            assert(0);
        }
        return newMarker();
    }
    /**
     * Free marker
     */
    inline void freeMarker( Marker m)
    {
        is_used[ m.index] = false;
    }
};

