<?

if (!defined("BASE")) {
    define ("BASE",true);

abstract class base
{
    protected $_v = array();
    protected $_enable_get = array();
    protected $_enable_set = array();
    protected $image_type = array(
                                    1 => 'gif',
                                    2 => 'jpeg',
                                    3 => 'png',
                                  );


	abstract public function init();

    public function __get($nm)
    {
        if (array_search($nm, $this->_enable_get)!==false) {
            $r = $this->_v[$nm];
            return $r;
        } else {
            throw new Exception('Tried to get unknown property '.get_class($this).'::'.$nm);
        }
    }

    public function __set($nm, $val)
    {
        if (array_search($nm, $this->_enable_set)!==false) {
            $this->_v[$nm] = $val;
        } else {
            throw new Exception('Tried to set unknown property '.get_class($this).'::'.$nm);
        }
    }

    public function toArray()
    {
        return $this->_v;
    }

    public function LoadItem(Array $db_record) {
        reset($this->_v);
        while(list($key, $val)=each($this->_v)) {
            if (array_key_exists($key, $db_record)) $this->_v[$key] = $db_record[$key];
        }
    }

   public function CheckImageType($temp_file) {
      $image_info = getimagesize($temp_file);
      if (array_key_exists($image_info[2], $this->image_type)) {
         return true;
      }

      return false;
   }


   public function MakePreview($temp_file, $max_width, $dest_preview, $dest_orig='') {
      $img_info = getimagesize($temp_file);
      $func = 'imagecreatefrom'.$this->image_type[$img_info[2]];
      $big_im = $func($temp_file);

      $width = $img_info[0];
      $height = $img_info[1];

      if ($width>$max_width) {
         $coeff = $img_info[0]/$max_width;
         $width = $max_width;
         $height = $img_info[1]/$coeff;

         $small_im = imagecreatetruecolor($max_width, $height);
         imagecopyresampled($small_im, $big_im, 0, 0, 0, 0, $width, $height, $img_info[0], $img_info[1]);
      } else {
         $small_im = $big_im;
      }

      if ($dest_orig) {
         if (file_exists($dest_orig)) unlink($dest_orig);
         $path = dirname($dest_orig).'/';
         if (!file_exists($path)) mkdir($path, $this->_default_mask);

         copy($temp_file,$dest_orig);
      }

      if (file_exists($dest_preview)) unlink($dest_preview);
      $path = dirname($dest_preview).'/';
      if (!file_exists($path)) mkdir($path, $this->_default_mask);

      imagejpeg ($small_im, $dest_preview);
   }
}
}