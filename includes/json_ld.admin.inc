<?php
/**
 * @file
 * Admin File for JSON-LD.
 */

/**
 * JSON - LD Settings Form.
 */
function json_ld_settings_form() {

  // Group filter.
  $form['site'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Wide Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Site Name.
  $form['site']['json_ld_site_name_default'] = array(
    '#type' =>'checkbox',
    '#title' => t('Use the default site name'),
    '#default_value' => variable_get('json_ld_site_name_default', TRUE),
  );

  $form['site']['json_ld_site_name_custom'] = array(
    '#type' =>'textfield',
    '#title' => t('Custom site name'),
    '#default_value' => variable_get('json_ld_site_name_custom', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="json_ld_site_name_default"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Logo settings.
  $logo = theme_get_setting('logo');
  $current_logo = array(
    'path' => $logo,
    'attributes' => array()
  );
  $form['site']['json_ld_site_logo'] = array(
    '#type' =>'item',
    '#title' => t('The current site logo'),
    '#markup' => theme_image($current_logo),
  );

  // Group filter.
  $form['bz'] = array(
    '#type' => 'fieldset',
    '#title' => t('Business Info Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Business Type.
  $form['bz']['json_ld_business_type'] = array(
    '#type' => 'select',
    '#title' => t('Schema Org Type'),
    '#options' => array(
      'Organization' => 'Organization',
      'Airline' => 'Airline',
      'Corporation' => 'Corporation',
      'EducationalOrganization' => 'Educational Organization',
      'GovernmentOrganization' => 'Government Organization',
      'LocalBusiness' => 'Local Business',
      'MedicalOrganization' => 'Medical Organization',
      'NGO' => 'NGO',
      'PerformingGroup' => 'Performing Group',
      'SportsOrganization' => 'Sports Organization',
    ),
    '#empty_option' => t('- None - '),
    '#default_value' => variable_get('json_ld_business_type', ''),
  );

  // Address fields.
  $form['bz']['json_ld_address'] = array(
    '#type' =>'textfield',
    '#title' => t('Address'),
    '#default_value' => variable_get('json_ld_address', ''),
  );

  $form['bz']['json_ld_city'] = array(
    '#type' =>'textfield',
    '#title' => t('City'),
    '#default_value' => variable_get('json_ld_city', ''),
  );

  $form['bz']['json_ld_state'] = array(
    '#type' =>'textfield',
    '#title' => t('State'),
    '#default_value' => variable_get('json_ld_state', ''),
  );

  $form['bz']['json_ld_zip'] = array(
    '#type' =>'textfield',
    '#title' => t('Zip'),
    '#default_value' => variable_get('json_ld_zip', ''),
  );

  $form['bz']['json_ld_phone'] = array(
    '#type' =>'textfield',
    '#title' => t('Phone'),
    '#default_value' => variable_get('json_ld_phone', ''),
  );

  // Set the header.
  $header = array(
    'type' => t('Node Type'),
    'schema' => t('Schema Type'),
    'content' => t('Content Field'),
  );

  // Grab all the node types.
  $types = node_type_get_types();
  foreach ($types as $type) {
    // Schema select.
    $schema = array(
      '#type' => 'select',
      '#options' => array(
        '' => (' - None - '),
        'Article' => t('Article'),
        'Blog' => t('Blog'),
        'CollectionPage' => t('Collection Page'),
        'Book' => t('Book'),
        'Event' => t('Event'),
        'LocalBusiness' => t('Local Business'),
        'Organization' => t('Organization'),
        'Person' => t('Person'),
        'Product' => t('Product'),
        'Review' => t('Review'),
      ),
      '#name' => 'schema[json_ld_schema_' . $type->type . ']',
      '#value' => variable_get('json_ld_schema_' . $type->type, ''),
    );
    // Clear out vars to be safe and happy.
    $content = $fields = $field_options = array();

    // Grab the fields.
    $fields = field_info_instances('node', $type->type);
    // Go through and set this up.
    if (count($fields) > 0) {
      $field_options[] = (' - None - ');
      foreach ($fields as $field) {
        $field_options[] = $field['field_name'];
      }
      // Schema select.
      $content = array(
        '#type' => 'select',
        '#options' => drupal_map_assoc($field_options),
        '#name' => 'content[json_ld_content_' . $type->type . ']',
        '#value' => variable_get('json_ld_content_' . $type->type, ''),
      );
    }
    else {
      $content = array(
        '#type' =>'item',
        '#markup' => t('No Fields to Use.'),
      );
    }

    $row['json_ld_node_type_' . $type->type] = array(
      'type' => t($type->type),
      'schema' => array('data' => $schema),
      'content' => array('data' => $content),
    );
  }

  // Default values for the table.
  $query = db_query("SELECT * FROM {variable} WHERE name LIKE 'json_ld_node_type%'");
  foreach ($query->fetchAll() as $default) {
    if (isset($default->value) && unserialize($default->value)) {
      $table_defaults[$default->name] = TRUE;
    }
  }

  // Set the table select.
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $row,
    '#default_value' => $table_defaults,
    '#empty' => t('No Content Types.'),
  );

  // Set the custom #name data so we can win it in the submit later.
  $form['schema'] = array(
    '#type' => 'value',
  );
  $form['content'] = array(
    '#type' => 'value',
  );

  // Custom submit handler.
  $form['#submit'][] = '_json_ld_form_submit';

  return system_settings_form($form);
}

/**
 * Custom submit handler for json_ld_settings_form
 */
function _json_ld_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $key => $values) {
    switch ($key) {
      case 'table':
        foreach ($values as $name => $value) {
          if ($value) {
            variable_set($name, TRUE);
          }
          else {
            variable_del($name);
          }
        }
        break;
      case 'schema':
        foreach ($values as $name => $value) {
          if (!empty($value)) {
            variable_set($name, $value);
          }
          else {
            variable_del($name);
          }
        }
        break;
      case 'content':
        foreach ($values as $name => $value) {
          if ($value != '- None -') {
            variable_set($name, $value);
          }
          else {
            variable_del($name);
          }
        }
        break;
    }
  }
}
