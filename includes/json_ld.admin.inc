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

  // Site Name / Company Name.
  $form['site']['json_ld_site_name'] = array(
    '#type' =>'textfield',
    '#title' => t('Site Name / Company Name'),
    '#default_value' => variable_get('json_ld_site_name', ''),
    '#description' => t('The Company Name or the Site Name you want the site to identify with.'),
    '#required' => TRUE,
  );

  $form['site']['json_ld_site_desc'] = array(
    '#type' =>'textfield',
    '#title' => t('Site Default Description'),
    '#default_value' => variable_get('json_ld_site_desc', ''),
    '#description' => t('Default description for the site, similar to metatag description.'),
    '#maxlength' => 1024,
    '#required' => TRUE,
  );

  // Logo settings.
  $form['site']['logo_large'] = array(
    '#name' => 'block_image',
    '#type' => 'managed_file',
    '#title' => t('Upload Your Large Image File (non logo)'),
    '#description' => t('Width should be at least 696px!!  Upload a file, allowed extensions: jpg, jpeg, png, gif.'),
    '#default_value' => variable_get('json_ld_site_logo_large', ''),
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
    '#required' => TRUE,
  );

    // Logo settings.
  $form['site']['logo_small'] = array(
    '#name' => 'block_image',
    '#type' => 'managed_file',
    '#title' => t('Upload Your Default Logo File'),
    '#description' => t('Ideally, logos are exactly 60px tall with width <= 600px.  Logos with a height > 60px or width > 600px will be rejected by Google.  Upload a file, allowed extensions: jpg, jpeg, png, gif.'),
    '#default_value' => variable_get('json_ld_site_logo_small', ''),
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
    '#required' => TRUE,
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
      'GeneralContractor' => 'General Contractor',
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
  $node_header = array(
    'type' => t('Node Type'),
    'schema' => t('Node Schema Type'),
    'content' => t('Node Content Field'),
    'image' => t('Node Image Field'),
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
        'BlogPosting' => t('Blog Posting'),
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
    $content = $fields = $field_options = $image_options = array();

    // Grab the fields.
    $fields = field_info_instances('node', $type->type);
    // Go through and set this up.
    if (count($fields) > 0) {
      $field_options[] = $image_options[] = (' - None - ');
      foreach ($fields as $field) {
        // Grab the widget and win this.
        $widget = field_info_widget_types($field['widget']['type']);
        if ($widget['module'] == 'image' || $widget['module'] == 'media') {
          // Image types only.
          $image_options[] = $field['field_name'];
        }
        else {
          // For the field select list.
          $field_options[] = $field['field_name'];
        }
      }
      // Content select.
      $content = array(
        '#type' => 'select',
        '#options' => drupal_map_assoc($field_options),
        '#name' => 'content[json_ld_content_' . $type->type . ']',
        '#value' => variable_get('json_ld_content_' . $type->type, ''),
      );

      // Image select.
      $image = array(
        '#type' => 'select',
        '#options' => drupal_map_assoc($image_options),
        '#name' => 'image[json_ld_image_' . $type->type . ']',
        '#value' => variable_get('json_ld_image_' . $type->type, ''),
      );
    }
    else {
      $content = array(
        '#type' =>'item',
        '#markup' => t('No Fields to Use.'),
      );
      $image = array(
        '#type' =>'item',
        '#markup' => t('No Fields to Use.'),
      );
    }

    $row_node['json_ld_node_type_' . $type->type] = array(
      'type' => t($type->type),
      'schema' => array('data' => $schema),
      'content' => array('data' => $content),
      'image' => array('data' => $image),
    );
  }

  // Default values for the table.
  $table_defaults = array();
  $query = db_query("SELECT * FROM {variable} WHERE name LIKE 'json_ld_node_type%'");
  foreach ($query->fetchAll() as $default) {
    if (isset($default->value) && unserialize($default->value)) {
      $table_defaults[$default->name] = TRUE;
    }
  }

  // Node Type filter.
  $form['nt'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node Types'),
    '#description' => t('In order for the node type fields to be processed, you must select the checkbox.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Set the table select.
  $form['nt']['node_table'] = array(
    '#title' => t('Node Types'),
    '#type' => 'tableselect',
    '#header' => $node_header,
    '#options' => $row_node,
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
  $form['image'] = array(
    '#type' => 'value',
  );


  // Win the Views now.
  $views = views_get_all_views();

  $view_header = array(
    'name' => t('View Name'),
    'machine_name' => t('View Machine Name'),
    'display' => t('Display'),
  );
  // Win the view table and displays.
  foreach ($views as $view) {
    if (!$view->disabled) {
      if (isset($view->display)) {
        foreach ($view->display as $dname => $display) {
          $row_view['json_ld_view_' . $view->name . '_' . $dname] = array(
            'name' => $view->human_name,
            'machine_name' => $view->name,
            'display' => $display->display_title,
          );
        }
      }
    }
  }

  // Default values for the table.
  $view_defaults = array();
  $query = db_query("SELECT * FROM {variable} WHERE name LIKE 'json_ld_view%'");
  foreach ($query->fetchAll() as $default) {
    if (isset($default->value) && unserialize($default->value)) {
      $view_defaults[$default->name] = TRUE;
    }
  }

  // Node Type filter.
  $form['vw'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views'),
    '#description' => t('In order for this to work, the view must have nid OR title fields && the node type form which the view is derived from must be enabled and configured above.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Set the table select.
  $form['vw']['views_table'] = array(
    '#title' => t('Node Types'),
    '#type' => 'tableselect',
    '#header' => $view_header,
    '#options' => $row_view,
    '#default_value' => $view_defaults,
    '#empty' => t('No Content Types.'),
  );


  // Custom submit handler.
  $form['#submit'][] = '_json_ld_form_submit';

  return system_settings_form($form);
}

/**
 * Custom submit handler for json_ld_settings_form
 */
function _json_ld_form_submit($form, &$form_state) {
  // Go through and win each value for use later.
  foreach ($form_state['values'] as $key => $values) {
    switch ($key) {
      // Save the File.
      case 'logo_small':
      case 'logo_large':
        global $user;
        $file = file_load($values);
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
        file_usage_add($file, 'json_ld', 'logo', $user->uid);
        variable_set('json_ld_site_' . $key, $file->fid);
        break;
      case 'node_table':
      case 'views_table':
        // Set variables to use later.
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
      case 'content':
      case 'image':
        // Set variables to use later.
        foreach ($values as $name => $value) {
          $value = trim($value);
          if ($value != '- None -' && !empty($value)) {
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
