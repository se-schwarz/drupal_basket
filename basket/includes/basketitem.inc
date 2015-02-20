<?php

class BasketItem {
  public $identificator = null;
  public $id = null;
  public $type = null;
  public $object = null;
  public $amount = 1;

  /**
   * Constructor
   * @param $type
   * @param $id
   */
  public function __construct($type, $id) {
    $this->identificator = BasketItem::create_identificator($type, $id);
    $this->type = $type;
    $this->id = $id;
    $this->load();
  }

  /**
   * Creates an unique identificator for the basket item
   * @param $type
   * @param $id
   * @return string
   */
  public static function create_identificator($type, $id) {
    return md5($type.'__'.$id);
  }

  /**
   * Returns the identificator of the item
   * @return string
   */
  public function get_identificator() {
    return $this->identificator;
  }

  public function get_id() {
    return $this->id;
  }

  public function get_type() {
    return $this->type;
  }

  public function set_amount($amount) {
    $this->amount = $amount;
  }

  public function get_amount() {
    return $this->amount;
  }

  public function get_object() {
    if ($this->object == null)
      $this->load();
    return $this->object;
  }

  public function load() {
    if ($this->type == null || $this->id == null) return null;
    $list = entity_load($this->type, array($this->id));
    if (count($list) > 0) {
      $this->object = $list[$this->id];
    } else {
      $this->object = null;
    }
  }
}