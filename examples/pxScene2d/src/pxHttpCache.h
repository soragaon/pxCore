#ifndef _PX_HTTPCACHE
#define _PX_HTTPCACHE

#include "rtFile.h"
#include "rtString.h"
#include "rtValue.h"
#include <map>
#include <vector>
#include <time.h>

class rtHttpCacheData
{
  public:
    rtHttpCacheData();
    rtHttpCacheData(const char* url);
    rtHttpCacheData(const char* url, const char* headerMetadata, const char* data, int size=0);

    ~rtHttpCacheData();
    /* returns the expiration date of the cache data in localtime */
    rtString expirationDate();
    time_t expirationDateUnix() const;

    /* returns true if cache data is expired */
    bool isExpired(); 

    /* returns true if data exists in the cache and the cache is not expired */
    bool isValid();

    /* returns true if the cache data can be saved to disk (not expired,no-store is not in cache control etc) */
    bool isWritableToCache();

    /* sets the attributes.  the rawAttributes string contains the headers string. */
    void setAttributes(char* rawAttributes);

    /* returns a map of all the headers associated with the cached data */
    rtError attributes(std::map<rtString, rtString>& cacheAttributes);

    /* returns the file data in the cache.  This is a blocking call and will check the network for updated data if etag is used */
    rtError data(rtData& data);

    /* sets the image file data to be stored in cache */
    void setData(rtData& cacheData);

    /* returns the url associated with the cache */
    rtError url(rtString& url) const;

    /* returns the Etag associated with the data */
    rtError etag(rtString& tag);

    /* returns true if server has updated the image data between requests */
    bool isUpdated();

    /* returns header data */
    rtData& getHeaderData();

    /* returns image data */
    rtData& getContentsData();

    void setFilePointer(FILE* fp);

  private:
    /* populates the map with header attribute and value */
    void populateHeaderMap();
 
    /* set the expiration date of cache data based on max-age and expires field in header */
    void setExpirationDate();

    /* calculate  any revalidation of cache data is needed based on no-cache, must-revalidate parameters */
    rtError calculateRevalidationNeed(bool&,bool&);

    /* initiate and handle download */
    bool handleDownloadRequest(std::vector<rtString>& headers,bool downloadBody=true);

    /* read the file data and populate it in mData. returns true on sucess and false on failure/empty data */
    bool readFileData();

    /* read the file and populate the previously calculated expiration date */
    void populateExpirationDateFromCache();

    /* perform revalidation of entire response by querying the server and populating new data */
    rtError performRevalidation(rtData& data);

    /* perform revalidation of header by querying the server and populating new data */
    rtError performHeaderRevalidation();

    /* perform etag handling */
    rtError handleEtag(rtData& data);

    rtString mUrl;
    rtData mHeaderMetaData;
    rtData mData;
    std::map<rtString, rtString> mHeaderMap;
    time_t mExpirationDate;
    FILE* fp;
    bool mUpdated;

};
#endif
