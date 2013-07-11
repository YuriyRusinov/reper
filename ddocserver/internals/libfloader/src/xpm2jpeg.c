/*#include <>*/
/*

Функция xpm2jpg конвертирует данные формата XPM в данные формата JPEG
*/

#include "floader.h"

#include <jpeglib.h>
#include <jerror.h>

#define read_long(str, value)                                               \
{                                                                           \
    char *endptr;                                                           \
    errno = 0;                                                              \
    (value) = strtol(str, &endptr, 10);                                     \
    if ((errno == ERANGE && ((value) == LONG_MAX || (value) == LONG_MIN))   \
        || (errno != 0 && (value) == 0))                                    \
    {                                                                       \
        return -1;                                                          \
    }                                                                       \
    if (endptr == (str))                                                    \
        return -1;                                                          \
    if (*endptr != '\0')                                                    \
        (str) = endptr;                                                     \
}

/* Basic XPM parser */

static int xpm_parse(char *xpm, unsigned long xpmsize,
                     char **xpmstrings[], unsigned long *numstrings)
{
    unsigned long i, nstrings, nchars;
    char *cur;

    nstrings = 128;
    *xpmstrings = (char **)calloc(nstrings, sizeof(char *));

    /* Skip to the first opening quote. Assume it is a start of data
     * array. */
    cur = strchr((char *)xpm, '\"');

    i = 0;
    nchars = 0;
    while (cur < xpm + xpmsize)
    {
        if (*cur == '\"')
        {
            cur++;
            continue;
        }
        else if (*cur == '\n')
        {
            if (i >= nstrings)
            {
                nstrings *= 2;
                *xpmstrings =
                    (char **) realloc(*xpmstrings, nstrings * sizeof(char *));
            }

            (*xpmstrings)[i] = (char *)calloc(nchars + 1, sizeof(char));
            memcpy((*xpmstrings)[i], cur - nchars - 1, nchars);
            cur++;
            i++;
            nchars = 0;
            continue;
        }
        nchars++;
        cur++;
    }

    *numstrings = i;

    return 0;
}

/* Parse the XPM structure and return back the raw RGB data array */

static int xpm2raw(char *xpm, unsigned long xpmsize,
                   unsigned char **pimage, long *pwidth, long *pheight)
{
    long    width, height, numcolors, cpp;
    long    i;
    char    *str;
    char **xpmstrings;
    unsigned long numstrings;
    unsigned char *colortable;
    unsigned long *symboltable;

    xpm_parse(xpm, xpmsize, &xpmstrings, &numstrings);

    /* Read XPM header */
    str = xpmstrings[0];
    if (!str || strlen(str) == 0)
        return -1;

    read_long(str, width);
    read_long(str, height);
    read_long(str, numcolors);
    read_long(str, cpp);

    /* Read color table */
    colortable = (unsigned char *)calloc(3 * numcolors, sizeof(unsigned char));
    symboltable = (unsigned long *)calloc(numcolors, sizeof(unsigned long));
    for (i = 0; i < numcolors; i++)
    {
        long j;

        str = xpmstrings[i + 1];
        for (j = 0; j < cpp; j++)
            symboltable[i] += (unsigned long)str[j] << j*8;
        str = strchr(str + cpp, '#');
        if (str)
        {
            unsigned int red, green, blue;
            sscanf(str, "#%02x%02x%02x", &red, &green, &blue);
            colortable[3 * i] = (unsigned char)red;
            colortable[3 * i + 1] = (unsigned char)green;
            colortable[3 * i + 2] = (unsigned char)blue;
        }
        else
        {
            colortable[3 * i] = 255;
            colortable[3 * i + 1] = 255;
            colortable[3 * i + 2] = 255;
        }
    }

    /* Read image data */
    *pwidth = width;
    *pheight = height;
    *pimage = (unsigned char *)calloc(3 * width * height, sizeof(unsigned char));
    for (i = 0; i < height; i++)
    {
        unsigned long symbol;
        long pixel, j;

        str = xpmstrings[i + numcolors + 1];

        for (pixel = 0; pixel < width; pixel++)
        {
            symbol = 0;
            for (j = 0; j < cpp; j++)
                symbol += (unsigned long)str[pixel * cpp + j] << j*8;
            for (j = 0; j < numcolors; j++)
            {
                if (symboltable[j] == symbol)
                {
                    memcpy(*pimage + 3 * (i * width + pixel), colortable + 3 * j, 3);
                    break;
                }
            }
        }
    }

    /* Free allocated resources */
    if (colortable)
        free(colortable);
    if (symboltable)
        free(symboltable);
    if (xpmstrings)
    {
        for (i = 0; i < numstrings; i++)
            free(xpmstrings[i]);
        free(xpmstrings);
    }

    return 0;
}

#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */

/* Expanded data destination object for memory output */

typedef struct {
  struct jpeg_destination_mgr pub; /* public fields */

  unsigned char ** outbuffer;	/* target buffer */
  unsigned long * outsize;
  unsigned char * newbuffer;	/* newly allocated buffer */
  JOCTET * buffer;		/* start of buffer */
  size_t bufsize;
} my_mem_destination_mgr;

typedef my_mem_destination_mgr * my_mem_dest_ptr;

static void init_mem_destination (j_compress_ptr cinfo)
{
  /* no work necessary here */
}

static boolean empty_mem_output_buffer (j_compress_ptr cinfo)
{
  size_t nextsize;
  JOCTET * nextbuffer;
  my_mem_dest_ptr dest = (my_mem_dest_ptr) cinfo->dest;

  /* Try to allocate new buffer with double size */
  nextsize = dest->bufsize * 2;
  nextbuffer = malloc(nextsize);

  if (nextbuffer == NULL)
    ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 10);

  memcpy(nextbuffer, dest->buffer, dest->bufsize);

  if (dest->newbuffer != NULL)
    free(dest->newbuffer);

  dest->newbuffer = nextbuffer;

  dest->pub.next_output_byte = nextbuffer + dest->bufsize;
  dest->pub.free_in_buffer = dest->bufsize;

  dest->buffer = nextbuffer;
  dest->bufsize = nextsize;

  return TRUE;
}

static void term_mem_destination (j_compress_ptr cinfo)
{
  my_mem_dest_ptr dest = (my_mem_dest_ptr) cinfo->dest;

  *dest->outbuffer = dest->buffer;
  *dest->outsize = dest->bufsize - dest->pub.free_in_buffer;
}

/*static*/ void jpeg_mem_dest (j_compress_ptr cinfo,
                           unsigned char ** outbuffer, unsigned long * outsize)
{
  my_mem_dest_ptr dest;

  if (outbuffer == NULL || outsize == NULL)	/* sanity check */
    ERREXIT(cinfo, JERR_BUFFER_SIZE);

  /* The destination object is made permanent so that multiple JPEG images
   * can be written to the same buffer without re-executing jpeg_mem_dest.
   */
  if (cinfo->dest == NULL) {	/* first time for this JPEG object? */
    cinfo->dest = (struct jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_mem_destination_mgr));
  }

  dest = (my_mem_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_mem_destination;
  dest->pub.empty_output_buffer = empty_mem_output_buffer;
  dest->pub.term_destination = term_mem_destination;
  dest->outbuffer = outbuffer;
  dest->outsize = outsize;
  dest->newbuffer = NULL;

  if (*outbuffer == NULL || *outsize == 0) {
    /* Allocate initial buffer */
    dest->newbuffer = *outbuffer = malloc(OUTPUT_BUF_SIZE);
    if (dest->newbuffer == NULL)
      ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 10);
    *outsize = OUTPUT_BUF_SIZE;
  }

  dest->pub.next_output_byte = dest->buffer = *outbuffer;
  dest->pub.free_in_buffer = dest->bufsize = *outsize;
}

int xpm2jpeg(char *xpm, unsigned long xpmsize,
             unsigned char **pjpeg, unsigned long *pjpegsize)
{
    unsigned char   *image;
    long            width, height, row;
    unsigned char   *outbuffer = NULL;
    unsigned long   outsize = 0;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* Read XPM image in raw data array */
    if (xpm2raw(xpm, xpmsize, &image, &width, &height) < 0)
    {
        return -1;
    }

    /* Initialize the JPEG compression object with default error handling. */
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    /* Initialize JPEG parameters.
    * Much of this may be overridden later.
    * In particular, we don't yet know the input file's color space,
    * but we need to provide some value for jpeg_set_defaults() to work.
    */

    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    cinfo.input_components = 3;
    cinfo.data_precision = 8;
    cinfo.image_height = height;
    cinfo.image_width = width;

    jpeg_mem_dest(&cinfo, &outbuffer, &outsize);

    /* Start compressor */
    jpeg_start_compress(&cinfo, TRUE);

    /* Process data */
    for (row = 0; row < height; row++)
    {
        JSAMPROW bufptr[1];

        bufptr[0] = (JSAMPROW) (image + row * width * 3);
        jpeg_write_scanlines(&cinfo, bufptr, 1);
    }

    /* Finish compression and release memory */
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    if (image)
        free(image);

    *pjpeg = outbuffer;
    *pjpegsize = outsize;

    return 0;
}

