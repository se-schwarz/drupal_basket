<?php

require_once(dirname(__FILE__).'/basketitem.inc');

class Basket {
  public $items = array();

  /**
   * This returns the basket object for further use.
   * The basket is always recreated so updates are loaded immediately.
   * @return Basket
   */
  public static function get_basket() {
    return new Basket();
  }

  /**
   * Returns the raw item count of the basket.
   * The items are not loaded, so this is much more efficient then count(Basket::get_basket()->get_objects()),
   * but it does not take deleted objects in consideration.
   * @return int
   */
  public static function get_item_count() {
    drupal_session_start();
    if (isset($_SESSION[BASKET_SESSION_VARIABLE])) {
      $items = unserialize($_SESSION[BASKET_SESSION_VARIABLE]);
      if (is_array($items))
        return count($items);
    }
    return 0;
  }

  public function __construct() {
    $this->load();
  }

  /**
   * Loads the basket from the session
   */
  public function load() {
    drupal_session_start();
    $item_to_remove = array();
    if (isset($_SESSION[BASKET_SESSION_VARIABLE])) {
      $items = unserialize($_SESSION[BASKET_SESSION_VARIABLE]);
      foreach($items as $item) {
        // Remove empty items from the basket
        if ($item['type'] == null || $item['id'] == null) continue;
        $basket_item = new BasketItem($item['type'], $item['id']);
        if (isset($item['amount']))
          $basket_item->set_amount($item['amount']);

        // Deleted objects that aren't available anymore
        if (is_null($basket_item->get_object())) {
          $item_to_remove[] = $basket_item;
        }
        $this->items[$basket_item->get_identificator()] = $basket_item;
      }
    }
    $i = 0;
    foreach($item_to_remove as $item) {
      $this->remove($item->type, $item->id);
      $i++;
      if ($i == count($item_to_remove)) {
        drupal_set_message('We are sorry, but we had to remove an item from your cart, its no longer available.', 'info');
      }
    }
  }

  /**
   * Saves the basket to the session
   */
  public function save() {
    $items = array();
    foreach($this->items as $item) {
      $items[] = array('type' => $item->get_type(), 'id' => $item->get_id(), 'amount' => $item->get_amount());
    }
    $_SESSION[BASKET_SESSION_VARIABLE] = serialize($items);
  }

  /**
   * Checks if an items already exists in the basket
   *
   * @param $identificator
   * @return bool
   */
  public function item_exists($identificator) {
    return isset($this->items[$identificator]);
  }


  /**
   * Returns an item by its given basket it
   * @param $id
   * @return bool
   */
  public function get_by_id($id) {
    if (!$this->item_exists($id)) {
      return false;
    }
    return $this->items[$id];
  }

  /**
   * Returns the specific item from the basket, or false if it doesn't exist.
   * @param $type
   * @param $id
   * @return bool
   */
  public function get_object($type, $id) {
    $identificator = BasketItem::create_identificator($type, $id);
    if (!$this->item_exists($identificator)) {
      return false;
    }
    return $this->items[$identificator];
  }

  /**
   * Returns an array of loaded objects
   * @return array
   */
  public function get_objects() {
    $objects = array();
    foreach($this->items as $item) {
      $object = $item->get_object();
      $object->is_basket_item = true;
      $object->basket_object = $item;
      $object->basket_object_type = $item->get_type();
      $objects[] = $object;
    }
    $variables = array(
      'objects' => $objects
    );
    drupal_alter('basket_get_objects', $variables);
    return $objects;
  }

  /**
   * Adds an item to the basket, returns the object if the item has been added.
   * Returns false if the item already existed.
   * @param $type
   * @param $id
   * @return bool|null
   */
  public function add($type, $id) {
    if ($this->item_exists(BasketItem::create_identificator($type, $id))) {
      return false;
    }
    // Item didn't exist, so create it
    $item = new BasketItem($type, $id);
    $this->items[$item->get_identificator()] = $item;
    $this->save();
    return $item->get_object();
  }

  /**
   * Removes an item from the basket and returns that item if it exists.
   * Returns false if an item is not in the basket or null if the item doesn't exist
   * @param $type
   * @param $id
   * @return null
   */
  public function remove($type, $id) {
    $identificator = BasketItem::create_identificator($type, $id);
    if ($this->item_exists($identificator)) {
      $item = $this->items[$identificator];
      $object = $item->get_object();
      unset($this->items[$identificator]);
      $this->save();
      return $object;
    }
    return false;
  }

  /**
   * Takes an array of add and remove instructiosn and executes them
   * @param $args
   * @return array
   */
  public function update($args) {
    $results = array('add' => array(), 'remove' => array());
    if (isset($args['add'])) {
      foreach($args['add'] as $add) {
        $result = $this->add($add['type'], $add['id']);
        if ($result !== false) $results['add'][] = $result;
      }
    }
    if (isset($args['remove'])) {
      foreach($args['remove'] as $remove) {
        $result = $this->remove($remove['type'], $remove['id']);
        if ($result !== false) $results['remove'][] = $result;
      }
    }
    return $results;
  }


  /**
   * Clears the basket
   */
  public function clear() {
    $this->items = array();
    $this->save();
  }
}