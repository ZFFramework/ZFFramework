/**
 * @page DocTag_Tutorial_UsingContainer Using container
 *
 * ZFFramework use these container types:
 * -  all other C++ containers, such as std::vector,
 *   can be used directly,
 *   but must not be exported in header
 * -  ZFCoreArray\n
 *   C++-style container similar to std::vector,
 *   may not so efficient compared to STL containers,
 *   used to reduce dependency,
 *   use only if necessary
 *   @code
 *     ZFCoreArray<zfstring> array;
 *     array.add("element");
 *     const zfstring &e = array[0];
 *   @endcode
 * -  ZFContainer/ZFKeyValueContainer\n
 *   container as ZFObject,
 *   can hold ZFObject types only\n
 *   typically we have ZFArray, ZFSet, ZFHashSet as container,
 *   and ZFMap, ZFHashMap as key value container\n
 *   all container would automatically retain elements when add,
 *   and release when remove
 *   @code
 *     zfobj<ZFArray> array;
 *
 *     // can hold ZFObject types only
 *     array->add(zfobj<v_zfstring>());
 *   @endcode
 */

